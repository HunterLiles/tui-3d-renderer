add_rules("mode.debug", "mode.release")

target("tui-framework")
set_kind("binary")
add_files("src/*.c")
