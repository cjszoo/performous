#!/bin/sh

env PERFORMOUS_DEFAULT_CONFIG_FILE=@PERFORMOUS_CONFIG_FILE@ PLUGIN_PATH=@PERFORMOUS_PLUGIN_PATH@ PERFORMOUS_THEME_PATH=@PERFORMOUS_THEME_PATH@ @PERFORMOUS_EXECUTABLE@ "$@"
