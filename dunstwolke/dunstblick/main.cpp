#include <SDL.h>
#include <SDL_image.h>
#include <sdl2++/renderer>
#include <vector>
#include <filesystem>

#include "widget.hpp"
#include "layouts.hpp"
#include "widgets.hpp"
#include "resources.hpp"

#include "inputstream.hpp"

#include "layoutparser.hpp"

static bool shutdown_app_requested = false;

[[noreturn]] static void exit_sdl_error(char const * msg = nullptr) {
	fprintf(
	      stderr,
	      "%s: %s\n",
	      (msg != nullptr) ? msg : "sdl error",
	      SDL_GetError()
	      );
	fflush(stdout);
	fflush(stderr);
	exit(1);
}

static std::unique_ptr<Widget> root_widget;
static std::unique_ptr<RenderContext> current_rc;

RenderContext & context() {
	return *current_rc;
}

static UIValue deserialize_value(UIType type, InputStream & stream)
{
	switch(type)
	{
		case UIType::enumeration:
			return stream.read_byte();

		case UIType::integer:
			return gsl::narrow<int>(stream.read_uint());

		case UIType::resource:
			return UIResourceID(stream.read_uint());

		case UIType::number:
			return gsl::narrow<float>(stream.read_float());

		case UIType::boolean:
			return (stream.read_byte() != 0);

		case UIType::size:
		{
			SDL_Size size;
			size.w = gsl::narrow<int>(stream.read_uint());
			size.h = gsl::narrow<int>(stream.read_uint());
			return size;
		}

		case UIType::string:
			return std::string(stream.read_string());

		case UIType::margins:
		{
			UIMargin margin(0);
			margin.left = gsl::narrow<int>(stream.read_uint());
			margin.top = gsl::narrow<int>(stream.read_uint());
			margin.right = gsl::narrow<int>(stream.read_uint());
			margin.bottom = gsl::narrow<int>(stream.read_uint());
			return margin;
		}

		case UIType::sizelist:
		{
			UISizeList list;

			auto len = stream.read_uint();

			list.resize(len);
			for(size_t i = 0; i < list.size(); i += 4)
			{
				uint8_t value = stream.read_byte();
				for(size_t j = 0; j < std::min(4UL, list.size() - i); j++)
				{
					switch((value >> (2 * j)) & 0x3)
					{
						case 0: list[i + j] = UISizeAutoTag { }; break;
						case 1: list[i + j] = UISizeExpandTag { }; break;
						case 2: list[i + j] = 0; break;
						case 3: list[i + j] = 1.0f; break;
					}
				}
			}

			for(size_t i = 0; i < list.size(); i++)
			{
				switch(list[i].index())
				{
					case 2: // pixels
						list[i] = int(stream.read_uint());
						break;
					case 3: // percentage
						list[i] = stream.read_float();
						break;
				}
			}

			return std::move(list);
		}

	}
	assert(false and "property type not in table yet!");
}

static std::unique_ptr<Widget> deserialize_widget(UIWidget widgetType, InputStream & stream)
{
	auto widget = Widget::create(widgetType);
	assert(widget);

	UIProperty property;
	do
	{
		property = stream.read_enum<UIProperty>();
		if(property != UIProperty::invalid)
		{
			auto const value = deserialize_value(getPropertyType(property), stream);
			widget->setProperty(property, value);
		}
	} while(property != UIProperty::invalid);

	UIWidget childType;
	do
	{
		childType = stream.read_enum<UIWidget>();
		if(childType != UIWidget::invalid)
			widget->children.emplace_back(deserialize_widget(childType, stream));
	} while(childType != UIWidget::invalid);

	return widget;
}

static std::unique_ptr<Widget> deserialize_widget(InputStream & stream)
{
	auto const widgetType = stream.read_enum<UIWidget>();
	return deserialize_widget(widgetType, stream);
}

static std::map<UIResourceID, Resource> resources;

xstd::optional<Resource const &> find_resource(UIResourceID id)
{
	if(auto it = resources.find(id); it != resources.end())
		return it->second;
	else
		return xstd::nullopt;
}

void set_resource(UIResourceID id, Resource && resource)
{
	if(auto it = resources.find(id); it != resources.end())
		it->second = std::move(resource);
	else
		resources.emplace(id, std::move(resource));
}

static SDL_Rect screen_rect = { 0, 0, 0, 0 };

static void update_layout()
{
	root_widget->updateWantedSize();
	root_widget->layout(screen_rect);
}

static void set_ui_root(UIResourceID id)
{
	if(auto resource = find_resource(id); resource)
	{
		if(resource->index() != int(ResourceKind::layout))
			throw std::runtime_error("invalid resource: wrong kind!");

		auto const & layout = std::get<LayoutResource>(*resource);

		InputStream stream = layout.get_stream();

		root_widget = deserialize_widget(stream);

		update_layout();
	}
}

#include <fstream>

int main()
{
	//////////////////////////////////////////////////////////////////////////////
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		exit_sdl_error();
	}
	atexit(SDL_Quit);

	if(TTF_Init() < 0) {
		exit_sdl_error();
	}
	atexit(TTF_Quit);

	if(IMG_Init(IMG_INIT_PNG) < 0) {
		exit_sdl_error();
	}
	atexit(IMG_Quit);

	//////////////////////////////////////////////////////////////////////////////

	SDL_Window * window = SDL_CreateWindow(
	      "DunstBlick Frontend",
	      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	      1280, 720,
	      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	      );
	if(window == nullptr) {
		exit_sdl_error();
	}

	SDL_GetWindowSize(window, &screen_rect.w, &screen_rect.h);

	current_rc = std::make_unique<RenderContext>(
		sdl2::renderer { window },
		"./fonts/Roboto-Regular.ttf",
		"./fonts/CrimsonPro-Regular.ttf",
		"./fonts/SourceCodePro-Regular.ttf"
	);

	context().renderer.setBlendMode(SDL_BLENDMODE_BLEND); // enable alpha blend

	//////////////////////////////////////////////////////////////////////////////




	printf("cwd = %s\n", std::filesystem::current_path().c_str());
	fflush(stdout);

	std::ifstream input_src("./dunstblick/development.uit");

	std::stringstream formDataBuffer;

	LayoutParser layout_parser;
	layout_parser.compile(input_src, formDataBuffer);

	auto formData = formDataBuffer.str();
	set_resource(UIResourceID(1), LayoutResource(reinterpret_cast<uint8_t const *>(formData.data()), formData.size()));

	auto * tex = IMG_LoadTexture(context().renderer, "./images/small-test.png");

	set_resource(UIResourceID(2), BitmapResource(sdl2::texture(std::move(tex))));

	//////////////////////////////////////////////////////////////////////////////

	set_ui_root(UIResourceID(1));

	while(not shutdown_app_requested)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			switch(e.type) {
				case SDL_QUIT:
					shutdown_app_requested = true;
					break;
				case SDL_KEYDOWN:
					if(e.key.keysym.sym == SDLK_ESCAPE)
						shutdown_app_requested = true;
					break;
				case SDL_WINDOWEVENT:
					switch(e.window.event)
					{
						case SDL_WINDOWEVENT_RESIZED:
							screen_rect.w = e.window.data1;
							screen_rect.h = e.window.data2;
							update_layout();
							break;
					}
					break;
			}
		}

		auto const windowFlags = SDL_GetWindowFlags(window);

		// draw UI when window is visible
		if((windowFlags & (SDL_WINDOW_MINIMIZED | SDL_WINDOW_HIDDEN)) == 0)
		{
			context().renderer.resetClipRect();
			assert(not context().renderer.isClipEnabled());

			context().renderer.setColor(0x00, 0x00, 0x00, 0xFF);
			context().renderer.fillRect(context().renderer.getViewport());

			if(root_widget)
				root_widget->paint();

			context().renderer.present();

			if((windowFlags & (SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS)) != 0)
			{
				// 60 FPS with focused window
				SDL_Delay(16);
			}
			else
			{
				// 30 FPS with window in backgound
				SDL_Delay(33);
			}
		}
		else
		{
			// slow update loop when window is not visible
			SDL_Delay(100);
		}

	}

	root_widget.reset();
	current_rc.reset();

	SDL_DestroyWindow(window);

	return 0;
}
