#ifndef PARSTOOLS_PARSERS_HPP
#define PARSTOOLS_PARSERS_HPP

#include "parser_base.hpp"

using null_parser  = StrParser<"NULL">;
using bitch_parser = StrParser<"bitch">;
using plus_parser  = CharParser<'+'>;
using ws_parser    = CharParser<' '>;

using big_parser = MultiParser<null_parser,
                               bitch_parser,
                               plus_parser,
                               ws_parser>;

using kekis = MultiParser<ws_parser,
                          bitch_parser,
                          ws_parser>;

using word_parser    = MultiParser<ws_parser,
                                ConditionalStrParser<is_word>,
                                ws_parser>;

#endif //PARSTOOLS_PARSERS_HPP
