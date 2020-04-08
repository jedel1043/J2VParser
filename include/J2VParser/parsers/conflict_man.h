#ifndef J2VPARSER_CONFLICT_MAN_H
#define J2VPARSER_CONFLICT_MAN_H

namespace J2VParser::parsers {
    enum class Conflict {
        kFirstFirstConflict,
        kFirstFollowConflict,
        kReduceReduceConflict,
        kShiftReduceConflict
    };
} // namespace compiler::parsers

#endif //J2VPARSER_CONFLICT_MAN_H
