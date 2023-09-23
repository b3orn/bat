define(`VERSION', eval(include(`BUILD_VERSION')))dnl
syscmd(`echo ' incr(VERSION)` > BUILD_VERSION')dnl
