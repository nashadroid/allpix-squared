/**
 * @file
 * @brief Template implementation of configuration
 * @copyright Copyright (c) 2017 CERN and the Allpix Squared authors.
 * This software is distributed under the terms of the MIT License, copied verbatim in the file "LICENSE.md".
 * In applying this license, CERN does not waive the privileges and immunities granted to it by virtue of its status as an
 * Intergovernmental Organization or submit itself to any jurisdiction.
 */

namespace allpix {
    /**
     * @throws MissingKeyError If the requested key is not defined
     * @throws InvalidKeyError If the conversion to the requested type did not succeed
     * @throws InvalidKeyError If an overflow happened while converting the key
     */
    template <typename T> T Configuration::get(const std::string& key) const {
        try {
            auto node = parse_value(config_.at(key));
            try {
                return allpix::from_string<T>(node->value);
            } catch(std::invalid_argument& e) {
                throw InvalidKeyError(key, getName(), node->value, typeid(T), e.what());
            }
        } catch(std::out_of_range& e) {
            throw MissingKeyError(key, getName());
        } catch(std::invalid_argument& e) {
            throw InvalidKeyError(key, getName(), config_.at(key), typeid(T), e.what());
        } catch(std::overflow_error& e) {
            throw InvalidKeyError(key, getName(), config_.at(key), typeid(T), e.what());
        }
    }
    /**
     * @throws InvalidKeyError If the conversion to the requested type did not succeed
     * @throws InvalidKeyError If an overflow happened while converting the key
     */
    template <typename T> T Configuration::get(const std::string& key, const T& def) const {
        if(has(key)) {
            return get<T>(key);
        }
        return def;
    }

    /**
     * @throws MissingKeyError If the requested key is not defined
     * @throws InvalidKeyError If the conversion to the requested type did not succeed
     * @throws InvalidKeyError If an overflow happened while converting the key
     */
    template <typename T> std::vector<T> Configuration::getArray(const std::string& key) const {
        try {
            std::string str = config_.at(key);

            std::vector<T> array;
            auto node = parse_value(str);
            for(auto& child : node->children) {
                try {
                    array.push_back(allpix::from_string<T>(child->value));
                } catch(std::invalid_argument& e) {
                    throw InvalidKeyError(key, getName(), child->value, typeid(T), e.what());
                }
            }
            return array;
        } catch(std::out_of_range& e) {
            throw MissingKeyError(key, getName());
        } catch(std::invalid_argument& e) {
            throw InvalidKeyError(key, getName(), config_.at(key), typeid(T), e.what());
        } catch(std::overflow_error& e) {
            throw InvalidKeyError(key, getName(), config_.at(key), typeid(T), e.what());
        }
    }
    /**
     * @throws InvalidKeyError If the conversion to the requested type did not succeed
     * @throws InvalidKeyError If an overflow happened while converting the key
     */
    template <typename T> std::vector<T> Configuration::getArray(const std::string& key, const std::vector<T> def) const {
        if(has(key)) {
            return getArray<T>(key);
        }
        return def;
    }

    /**
     * @throws MissingKeyError If the requested key is not defined
     * @throws InvalidKeyError If the conversion to the requested type did not succeed
     * @throws InvalidKeyError If an overflow happened while converting the key
     */
    template <typename T> Matrix<T> Configuration::getMatrix(const std::string& key) const {
        try {
            std::string str = config_.at(key);

            Matrix<T> matrix;
            auto node = parse_value(str);
            for(auto& child : node->children) {
                if(child->children.empty()) {
                    throw std::invalid_argument("matrix has less than two dimensions");
                }

                std::vector<T> array;
                // Create subarray of matrix
                for(auto& subchild : child->children) {
                    try {
                        array.push_back(allpix::from_string<T>(subchild->value));
                    } catch(std::invalid_argument& e) {
                        throw InvalidKeyError(key, getName(), subchild->value, typeid(T), e.what());
                    }
                }
                matrix.push_back(array);
            }
            return matrix;
        } catch(std::out_of_range& e) {
            throw MissingKeyError(key, getName());
        } catch(std::invalid_argument& e) {
            throw InvalidKeyError(key, getName(), config_.at(key), typeid(T), e.what());
        } catch(std::overflow_error& e) {
            throw InvalidKeyError(key, getName(), config_.at(key), typeid(T), e.what());
        }
    }

    /**
     * @throws InvalidKeyError If the conversion to the requested type did not succeed
     * @throws InvalidKeyError If an overflow happened while converting the key
     */
    template <typename T> Matrix<T> Configuration::getMatrix(const std::string& key, const Matrix<T> def) const {
        if(has(key)) {
            return getMatrix<T>(key);
        }
        return def;
    }

    template <typename T> void Configuration::set(const std::string& key, const T& val) {
        config_[key] = allpix::to_string(val);
    }
    template <typename T> void Configuration::setArray(const std::string& key, const std::vector<T>& val) {
        // NOTE: not the most elegant way to support arrays
        std::string str;
        for(auto& el : val) {
            str += ",";
            str += allpix::to_string(val);
        }
        set(key, str);
    }

    template <typename T> void Configuration::setDefault(const std::string& key, const T& val) {
        if(!has(key)) {
            set<T>(key, val);
        }
    }

    template <typename T> void Configuration::setDefaultArray(const std::string& key, const std::vector<T>& val) {
        if(!has(key)) {
            setArray<T>(key, val);
        }
    }
} // namespace allpix
