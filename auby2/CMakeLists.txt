project(auby2)

set(CMAKE_CXX_STANDARD 20)

add_library(auby2 STATIC
  src/auby.mm
  src/internal.cc
  src/generated.cc
  src/cocos.cc
)

target_include_directories(auby2 PUBLIC
  include
  include/cocos2dx
  include/cocos2dx/include
  include/cocos2dx/platform/ios
  include/cocos2dx/kazmath/include
)
target_link_libraries(auby2 PUBLIC "-framework UIKit -framework WebKit -framework AVFoundation -framework CoreFoundation -framework Foundation -framework CoreGraphics")
