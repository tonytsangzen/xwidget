add_rules("mode.debug", "mode.release")
add_requires("sdl2","zlib", "freetype", "libpng")
set_encodings("utf-8")

target("xwidget")
	set_kind("shared")
	set_languages("c++17")
	add_packages("sdl2", "zlib", "freetype", "libpng", {public = true})
	if is_plat("windows") then
		add_requires("pthreads4w")
		add_package("pthreads4w")
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
	after_build(function (target)
        os.cp("wdemo/res", target:targetdir())
    end)
