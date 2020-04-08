#ifndef J2VPARSER_BIMAP_H
#define J2VPARSER_BIMAP_H

#include <sstream>

namespace J2VParser::utils {

    template<typename T, typename U>
    class Bimap {
    private:
        using Tmap = std::map<T, U>;
        using Umap = std::map<U, T>;
        Tmap t_map_;
        Umap u_map_;
    public:
        Bimap() = default;

        explicit Bimap(std::vector<std::pair<T, U>> init_list) {
            for (const auto &[left_e, right_e] : init_list) {
                t_map_[left_e] = right_e;
                u_map_[right_e] = left_e;
            }
        }

        void insert(const T &t_input, const U &u_input) {
            t_map_[t_input] = u_input;
            u_map_[u_input] = t_input;
        }

        const T &at(const U &u_input) const {
            return u_map_.at(u_input);
        }

        const U &at(const T &rule) const {
            return t_map_.at(rule);
        }

        [[nodiscard]] size_t size() const { return u_map_.size(); }

        [[nodiscard]] size_t count(const T &t_input) const {
            return t_map_.count(t_input);
        }

        [[nodiscard]] size_t count(const U &u_input) const {
            return u_map_.count(u_input);
        }
    };
}
#endif //J2VPARSER_BIMAP_H
