load("//build_tools:macros.bzl", "header_generator", "dll_generator", "entrypoint_generator")

header_generator(  
  deps = [
    "//util", 
    "//types:headers", 
    "//lisp:headers", 
    "@libuv//:headers"
  ]
)

dll_generator(
  packages = [
    "types",
    "lisp",
  ],
  deps = [
    "@libuv//:libuv"
  ],
  linkopts = [
    "-DEFAULTLIB:User32.lib", 
    "-DEFAULTLIB:Advapi32.lib",  
    "-DEFAULTLIB:Kernel32.lib",
    "-DEFAULTLIB:Iphlpapi.lib",
    "-DEFAULTLIB:Psapi.lib",
    "-DEFAULTLIB:Userenv.lib",
  ]
)


