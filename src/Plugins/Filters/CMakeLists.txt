#
# This file is part of USBProxy.
#

project(usbproxy_filters C)
set(PACKAGE usbproxy_filters)

include_directories(${libusbproxy_SOURCE_DIR})
LIST(APPEND PLUGINS_LINK_LIBS USBProxy)

LIST(APPEND PLUGINS PacketFilter_Callback PacketFilter_KeyLogger PacketFilter_ROT13 PacketFilter_UDPHID PacketFilter_MassStorage PacketFilter_Power PacketFilter_StreamLog PacketFilter_Xbox PacketFilter_PSClassic PacketFilter_Switch PacketFilter_Xbox360 PacketFilter_NeoGeo PacketFilter_PS3 PacketFilter_TG16Mini)

find_package(PCAP)

if( ${PCAP_FOUND} )
	LIST(APPEND PLUGINS PacketFilter_PcapLogger)
	include_directories(${PCAP_INCLUDE_DIRS})
	LIST(APPEND PLUGINS_LINK_LIBS ${PCAP_LIBRARIES})
else( ${PCAP_FOUND} )
	if( USE_PCAP AND NOT ${PCAP_FOUND} )
		message( FATAL_ERROR
			"Cannot find libpcap, which is required for USE_PCAP")
	endif()
endif( ${PCAP_FOUND} )

foreach(plugin ${PLUGINS})
	add_library(${plugin} SHARED ${plugin}.cpp)
	set_source_files_properties(${plugin}.cpp PROPERTIES COMPILE_FLAGS  "-std=c++11 -pthread -Wall -O3")
	set_target_properties(${plugin} PROPERTIES PREFIX "")
	target_link_libraries(${plugin} ${PLUGINS_LINK_LIBS})
	install(TARGETS ${plugin}
        LIBRARY DESTINATION lib/USBProxy${LIB_SUFFIX}
        COMPONENT sharedlibs
        )
endforeach(plugin)
