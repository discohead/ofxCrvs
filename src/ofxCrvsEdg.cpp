//
//  ofxCrvsEdg.cpp
//  example_ofxCrvs
//
//  Created by Jared McFarland on 12/22/23.
//

#include "ofxCrvsEdg.hpp"
#include "ofxCrvsUtils.hpp"
#include "ofxCrvsCrv.h"

namespace ofxCrvs {

float Edg::length()
{
    return glm::distance(source, target);
}

glm::vec2 Edg::at(float pos)
{
    glm::vec2 point;
    point.x = ofMap(pos, 0, 1, source.x, target.x);
    point.y = ofMap(pos, 0, 1, source.y, target.y);
    return point;
}

glm::vec2 Edg::midpoint()
{
    return at(0.5);
}

Edg Edg::transformed()
{
    glm::vec2 transformedSource = Utils::transform(source, midpoint(), scale, translation, rotation);
    glm::vec2 transformedTarget = Utils::transform(target, midpoint(), scale, translation, rotation);
    return Edg(transformedSource, transformedTarget, resolution);
}

std::vector<glm::vec2> Edg::points()
{
    return points(resolution);
}

std::vector<glm::vec2> Edg::points(int numPoints)
{
    std::vector<glm::vec2> points;
    for (int i = 0; i < numPoints; ++i)
    {
        points.push_back(at((float)i / (float)numPoints));
    }
    return points;
}

float Edg::angle()
{
    return glm::atan(target.y - source.y, target.x - source.x);
}

glm::vec2 Edg::getPerpendicularPoint(glm::vec2 point, float magnitude)
{
    glm::vec2 dir = this->asVector();
    dir = glm::normalize(dir);
    dir = glm::rotate(dir, glm::half_pi<float>());
    dir *= magnitude;
    return point + dir;
}

std::vector<glm::vec2> Edg::getCrvPoints(Crv crv, int resolution)
{
    std::vector<glm::vec2> edgPoints = points(resolution);
    std::vector<glm::vec2> crvPoints;
    for (int i = 0; i < resolution; ++i) {
        float x = (float)i/(float)(resolution - 1.f);
        float mag = crv.yAt(x);
        crvPoints.push_back(getPerpendicularPoint(edgPoints[i], mag));
    }
    return crvPoints;
}

std::vector<glm::vec2> Edg::getCrvPoints(Crv crv)
{
    return getCrvPoints(crv, resolution);
}

glm::vec2 Edg::asVector()
{
    return target - source;
}

} // namespace ofxCrvs
