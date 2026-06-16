if(CONFIG_ZMK_LAYER_REPORTER)
  zephyr_library()
  zephyr_library_sources(src/layer_reporter.c)
endif()
