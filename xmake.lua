add_rules("mode.debug", "mode.release")
add_requires("zlib", "freetype", "libpng")
set_encodings("utf-8")
if is_plat("windows") then
	add_requires("pthreads4w","libsdl")
else
	add_requires("sdl2")
end

target("xwidget")
	set_kind("shared")
	set_languages("c++17")
	add_packages("zlib", "freetype", "libpng", {public = true})
	if is_plat("windows") then
		set_runtimes("MD")
		add_packages("pthreads4w", "libsdl", {public = true})
		add_includedirs("libs/windows", {public = true})
		add_rules("utils.symbols.export_all", {export_classes = true})
	else
		add_packages("sdl2", {public = true})
	end
	add_cxflags("-Wno-implicit-function-declaration","-Wno-c++11-narrowing", {public = true})
	add_files("libs/**.cc|**/FontDialog.cc", "libs/**.c")
	add_includedirs("libs/widget++/include", 
					"libs/x++/include",
					"libs/graph/include",
					"libs/x/include", 
					"libs/basic",
					"libs/font/include",
					"libs/mouse/include",
					"libs/tinyjson/include/", 
					"libs/object++/include/",
					{public = true})
	add_includedirs(
					"libs/gterminal/include",
					"libs/textgrid/include",
					"libs/textgrid/include/textgrid")

target("wdemo")
	set_kind("binary")
	add_deps("xwidget")
	add_files("wdemo/**.cc")
	add_defines("SDL_MAIN_HANDLED")
	after_build(function (target)
        os.cp("wdemo/res", target:targetdir())
    end)
