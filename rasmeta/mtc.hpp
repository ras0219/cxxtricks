#pragma once

#include "false_depend.hpp"

// Super Experimental Typeclasses support:
//  * Metatypeclasses are template structs with the prefix mtc_
//  * Instances are defined by specializing the mtc_<classname> struct and providing method implementations

namespace rasmeta {

  /////////////////////////////////
  // Metatypeclasses can specialize implicits<mtc> to provide implicit implementations for methods
  template<template<class>class mtc, class T>
  struct implicits { };

}