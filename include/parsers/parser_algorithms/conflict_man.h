#ifndef COMPILER_CONFLICT_MAN_H
#define COMPILER_CONFLICT_MAN_H

namespace compiler::parsers {
    class ConflictManager{
    protected:
        enum class Conflict {
            kFirstFirstConflict,
            kFirstFollowConflict,
            kReduceReduceConflict,
            kShiftReduceConflict
        };
    };

} // namespace compiler::parsers

#endif //COMPILER_CONFLICT_MAN_H
