#pragma once

// Super Experimental Typeclasses support:
//  * Metatypeclasses are template structs with the prefix mtc_
//  * Instances are defined by specializing the mtc_<classname> struct and providing method implementations

namespace rasmeta {

  // This is used to generate a false dependency for Metatypeclasses
  template<class T> struct ____false_depend { enum { value = false }; };

  /////////////////////////////////
  // Metatypeclasses can specialize implicits<mtc> to provide implicit implementations for methods
  template<template<class>class mtc, class T>
  struct implicits { };

}