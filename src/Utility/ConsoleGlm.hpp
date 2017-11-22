//
// Created by MTesseracT on 22-11-2017.
//

#ifndef VULKANENGINE_CONSOLEGLM_HPP
#define VULKANENGINE_CONSOLEGLM_HPP

#include <iostream>
#include <iomanip>

//#include <glm/glm.hpp>
//#include <glm/ext.hpp>

/* the functions:
 * std::ostream & operator << ( std::ostream & out, glm::vec3 v );
 * std::ostream & operator << ( std::ostream & out, glm::vec4 v );
 * std::ostream & operator << ( std::ostream & out, glm::quat q );
 * std::ostream & operator << ( std::ostream & out, glm::mat4 m );
 */



inline std::ostream & operator << ( std::ostream & out, const glm::ivec2 & v ) {
    out << "(" << v.x << "," << v.y << ")";
    return out;
}

inline std::ostream & operator << ( std::ostream & out, const glm::uvec3 & v ) {
    out << "(" << v.x << "," << v.y << v.z << ")";
    return out;
}

inline std::ostream & operator << ( std::ostream & out, const glm::vec2 & v ) {
    out << "(" << v.x << "," << v.y << ")";
    return out;
}

inline std::ostream & operator << ( std::ostream & out, const glm::vec3 & v ) {
    out << "(" << v.x << "," << v.y << "," << v.z << ")";
    return out;
}

inline std::ostream & operator << ( std::ostream & out, const glm::vec4 & v ) {
    out << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
    return out;
}

inline std::ostream & operator << ( std::ostream & out, const glm::quat & q ) {
    out << "(" << q.x << "," << q.y << "," << q.z << "," << q.w << ")";
    return out;
}

inline std::ostream & operator << ( std::ostream & out, const glm::mat4 & m ) {
    out.precision (1);
    for ( int r = 0; r < 4; r++ ) {
        for ( int c = 0; c < 4; c++ ) {
            out << std::showpos << std::fixed << m[c][r] << " ";
        }
        out << std::endl;
    }
    return out;
}
#endif //VULKANENGINE_CONSOLEGLM_HPP
