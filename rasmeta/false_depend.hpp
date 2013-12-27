#pragma once

namespace rasmeta {

  // This is used to generate a false dependency for Metatypeclasses
  template<class T> struct ____false_depend { enum { value = false }; };

}