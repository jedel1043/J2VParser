#ifndef J2VP_CONFLICT_MAN_H
#define J2VP_CONFLICT_MAN_H

namespace J2VParser::parsers {
    enum class Conflict {
        kFirstFirstConflict,
        kFirstFollowConflict,
        kReduceReduceConflict,
        kShiftReduceConflict
    };
} // namespace compiler::parsers

#endif //J2VP_CONFLICT_MAN_H
