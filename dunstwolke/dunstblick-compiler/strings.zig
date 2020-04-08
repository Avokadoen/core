pub const widget_types = .{
    .{ .widget = "Button", .type = .button },
    .{ .widget = "Label", .type = .label },
    .{ .widget = "ComboBox", .type = .combobox },
    .{ .widget = "TreeView", .type = .treeview },
    .{ .widget = "ListBox", .type = .listbox },
    .{ .widget = "Picture", .type = .picture },
    .{ .widget = "TextBox", .type = .textbox },
    .{ .widget = "CheckBox", .type = .checkbox },
    .{ .widget = "RadioButton", .type = .radiobutton },
    .{ .widget = "ScrollView", .type = .scrollview },
    .{ .widget = "ScrollBar", .type = .scrollbar },
    .{ .widget = "Slider", .type = .slider },
    .{ .widget = "ProgressBar", .type = .progressbar },
    .{ .widget = "SpinEdit", .type = .spinedit },
    .{ .widget = "Separator", .type = .separator },
    .{ .widget = "Spacer", .type = .spacer },
    .{ .widget = "Panel", .type = .panel },
    .{ .widget = "Container", .type = .container },
    .{ .widget = "TabLayout", .type = .tab_layout },
    .{ .widget = "CanvasLayout", .type = .canvas_layout },
    .{ .widget = "FlowLayout", .type = .flow_layout },
    .{ .widget = "GridLayout", .type = .grid_layout },
    .{ .widget = "DockLayout", .type = .dock_layout },
    .{ .widget = "StackLayout", .type = .stack_layout },
};

pub const properties = .{
    .{ .property = "horizontal-alignment", .value = .horizontalAlignment },
    .{ .property = "vertical-alignment", .value = .verticalAlignment },
    .{ .property = "margins", .value = .margins },
    .{ .property = "paddings", .value = .paddings },
    .{ .property = "dock-site", .value = .dockSite },
    .{ .property = "visibility", .value = .visibility },
    .{ .property = "size-hint", .value = .sizeHint },
    .{ .property = "font-family", .value = .fontFamily },
    .{ .property = "text", .value = .text },
    .{ .property = "minimum", .value = .minimum },
    .{ .property = "maximum", .value = .maximum },
    .{ .property = "value", .value = .value },
    .{ .property = "display-progress-style", .value = .displayProgressStyle },
    .{ .property = "is-checked", .value = .isChecked },
    .{ .property = "tab-title", .value = .tabTitle },
    .{ .property = "selected-index", .value = .selectedIndex },
    .{ .property = "columns", .value = .columns },
    .{ .property = "rows", .value = .rows },
    .{ .property = "left", .value = .left },
    .{ .property = "top", .value = .top },
    .{ .property = "enabled", .value = .enabled },
    .{ .property = "image-scaling", .value = .imageScaling },
    .{ .property = "image", .value = .image },
    .{ .property = "binding-context", .value = .bindingContext },
    .{ .property = "child-source", .value = .childSource },
    .{ .property = "child-template", .value = .childTemplate },
    .{ .property = "hit-test-visible", .value = .hitTestVisible },
    .{ .property = "on-click", .value = .onClick },
    .{ .property = "orientation", .value = .orientation },
    .{ .property = "widget-name", .value = .name },
};

pub const enumerations = .{
    .{ .enumeration = "none", .value = .none },
    .{ .enumeration = "left", .value = .left },
    .{ .enumeration = "center", .value = .center },
    .{ .enumeration = "right", .value = .right },
    .{ .enumeration = "top", .value = .top },
    .{ .enumeration = "middle", .value = .middle },
    .{ .enumeration = "bottom", .value = .bottom },
    .{ .enumeration = "stretch", .value = .stretch },
    .{ .enumeration = "expand", .value = .expand },
    .{ .enumeration = "auto", .value = ._auto },
    .{ .enumeration = "yesno", .value = .yesno },
    .{ .enumeration = "truefalse", .value = .truefalse },
    .{ .enumeration = "onoff", .value = .onoff },
    .{ .enumeration = "visible", .value = .visible },
    .{ .enumeration = "hidden", .value = .hidden },
    .{ .enumeration = "collapsed", .value = .collapsed },
    .{ .enumeration = "vertical", .value = .vertical },
    .{ .enumeration = "horizontal", .value = .horizontal },
    .{ .enumeration = "sans", .value = .sans },
    .{ .enumeration = "serif", .value = .serif },
    .{ .enumeration = "monospace", .value = .monospace },
    .{ .enumeration = "percent", .value = .percent },
    .{ .enumeration = "absolute", .value = .absolute },
    .{ .enumeration = "zoom", .value = .zoom },
    .{ .enumeration = "contain", .value = .contain },
    .{ .enumeration = "cover", .value = .cover },
};
