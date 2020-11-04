install_name_tool -id "@loader_path/libbass.dylib" ../../3dparty/BASS/libbass.dylib
install_name_tool -id "@rpath/libbass.dylib" ../../3dparty/BASS/libbass.dylib
install_name_tool -change ../../3dparty/BASS/libbass.dylib @loader_path/libbass.dylib ../../uamp
 install_name_tool -add_rpath "@loader_path/" ../../uamp
