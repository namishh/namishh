local wezterm = require("wezterm")

local function font_with_fallback(name, params)
  local names = { name }
  return wezterm.font_with_fallback(names, params)
end

local font_name = "Fantasque Sans M Nerd Font"

return {
  -- OpenGL for GPU acceleration, Software for CPU
  front_end = "OpenGL",
  cell_width = 0.9,

  -- Font config
  font = font_with_fallback(font_name),
  font_rules = {
    {
      italic = true,
      font = font_with_fallback(font_name, { italic = true }),
    },
    {
      italic = true,
      intensity = "Bold",
      font = font_with_fallback(font_name, { italic = true, bold = true }),
    },
    {
      intensity = "Bold",
      font = font_with_fallback(font_name, { bold = true }),
    },
    {
      italic = true,
      font = font_with_fallback(font_name, { italic = true }),
    },
  },
  initial_rows = 18,
  initial_cols = 85,
  dpi = 96.0,
  bold_brightens_ansi_colors = true,
  warn_about_missing_glyphs = false,
  font_size = 14,
  line_height = 1.25,

  -- Cursor style
  default_cursor_style = "BlinkingUnderline",

  -- X11
  enable_wayland = false,

  colors = {
    foreground = "#fbf1c7",
    background = "#222222",
    cursor_bg = "#fbf1c7",
    cursor_fg = "#222222",
    cursor_border = "#222222",
    selection_fg = "#222222",
    selection_bg = "#fbf1c7",
    scrollbar_thumb = "#fbf1c7",
    split = "#222222",
    ansi = { "#3c3836", "#ea6962", "#a9b665", "#e79a4e", "#7daea3", "#d3869b", "#89b482", "#d4be98" },
    brights = { "#3c3836", "#ea6962", "#a9b665", "#e79a4e", "#7daea3", "#d3869b", "#89b482", "#d4be98" },
    indexed = { [136] = "#fbf1c7" },
    tab_bar = {
      active_tab = {
        bg_color = "#181818",
        fg_color = "#fbf1c7",
        italic = true,
      },
      inactive_tab = { bg_color = "#181818", fg_color = "#3c3836" },
      inactive_tab_hover = { bg_color = "#3c3836", fg_color = "#181818" },
      new_tab = { bg_color = "#3c3836", fg_color = "#181818" },
      new_tab_hover = { bg_color = "#6791c9", fg_color = "#181818" },
    },
  },

  -- Padding
  window_padding = {
    left = 32,
    right = 32,
    top = 32,
    bottom = 32,
  },

  -- Tab Bar
  enable_tab_bar = true,
  hide_tab_bar_if_only_one_tab = true,
  show_tab_index_in_tab_bar = false,
  tab_bar_at_bottom = true,

  -- General
  automatically_reload_config = true,
  inactive_pane_hsb = { saturation = 1.0, brightness = 1.0 },
  window_background_opacity = 1.0,
  window_close_confirmation = "NeverPrompt",
  window_frame = { active_titlebar_bg = "#090909", font = font_with_fallback(font_name, { bold = true }) },
}
