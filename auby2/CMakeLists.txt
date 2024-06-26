project(auby2)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(deps/spdlog)

if(CATALYST)
  set(PLATFORM_INCLUDES include/cocos2dx/platform/mac)
else()
  set(PLATFORM_INCLUDES include/cocos2dx/platform/ios)
endif()

add_library(auby2 STATIC
  src/auby.mm
  src/internal.cc
  src/generated.cc
  src/cocos.cc
  src/gd.cc
)

target_include_directories(auby2 PUBLIC
  include
  include/cocos2dx
  include/cocos2dx/include
  ${PLATFORM_INCLUDES}
  include/cocos2dx/kazmath/include
  include/extensions
)

target_link_libraries(auby2 PUBLIC spdlog "-framework UIKit -framework WebKit -framework AVFoundation -framework CoreFoundation -framework Foundation -framework CoreGraphics")
