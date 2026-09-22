add_rules("mode.debug", "mode.release")

target("terminal-renderer")
set_kind("binary")
add_files("src/*.c")
