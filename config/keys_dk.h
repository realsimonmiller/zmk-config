#pragma once
#include <dt-bindings/zmk/keys.h>

// ZMK-style Danish key aliases (no KC_, uses &kp directly)

#define DK_AE   &kp SCLN    // Æ
#define DK_OSTR &kp QUOT    // Ø
#define DK_ARNG &kp LBRC    // Å

#define DK_SECT &kp GRAVE   // § (approximated)
#define DK_EXLM &kp N1      // !
#define DK_DQUO &kp N2      // "
#define DK_HASH &kp N3      // #
#define DK_CURR &kp N4      // ¤ (approx)
#define DK_PERC &kp N5      // %
#define DK_AMPR &kp N6      // &
#define DK_SLSH &kp N7      // /
#define DK_LPRN &kp N8      // (
#define DK_RPRN &kp N9      // )
#define DK_EQL  &kp N0      // = (approx)
#define DK_PLUS &kp MINUS   // +
#define DK_MINS &kp SLASH   // -
#define DK_COMM &kp COMMA   // ,
#define DK_DOT  &kp DOT     // .
