#ifndef LAYOUTS_HPP
#define LAYOUTS_HPP

#include "widget.hpp"

struct StackLayout : Widget
{
    StackDirection direction;

    explicit StackLayout(StackDirection dir = StackDirection::vertical);

    void paintWidget(RenderContext &, const SDL_Rect &) override;

    void layoutChildren(SDL_Rect const & childArea) override;

    SDL_Size calculateWantedSize() override;

    void setProperty(UIProperty property, UIValue value) override;
};

struct DockLayout : Widget
{
    std::vector<DockSite> dockSites;

    void paintWidget(RenderContext &, const SDL_Rect &) override;

    void layoutChildren(SDL_Rect const & childArea) override;

    SDL_Size calculateWantedSize() override;

    DockSite getDockSite(size_t index) const;

    void setDockSite(size_t index, DockSite site);

    void setProperty(UIProperty property, UIValue value) override;
};



#endif // LAYOUTS_HPP
