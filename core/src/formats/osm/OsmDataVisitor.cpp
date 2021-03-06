#include "entities/Node.hpp"
#include "entities/Way.hpp"
#include "entities/Area.hpp"
#include "entities/Relation.hpp"
#include "formats/osm/MultipolygonProcessor.hpp"
#include "formats/osm/OsmDataVisitor.hpp"
#include "utils/GeometryUtils.hpp"

#include <algorithm>
#include <vector>

using namespace utymap;
using namespace utymap::formats;
using namespace utymap::entities;
using namespace utymap::index;

std::unordered_set<std::string> OsmDataVisitor::AreaKeys
{
    "building",
    "building:part",
    "landuse",
    "amenity",
    "harbour",
    "historic",
    "leisure",
    "man_made",
    "military",
    "natural",
    "office",
    "place",
    "power",
    "public_transport",
    "shop",
    "sport",
    "tourism",
    "waterway",
    "wetland",
    "water",
    "aeroway",
    "addr:housenumber",
    "addr:housename"
};

std::unordered_set<std::string> OsmDataVisitor::FalseKeys
{
    "no", "No", "NO", "false", "False", "FALSE", "0"
};

void OsmDataVisitor::visitBounds(BoundingBox bbox)
{
    statistics.bounds++;
}

void OsmDataVisitor::visitNode(std::uint64_t id, GeoCoordinate& coordinate, utymap::formats::Tags& tags)
{
    nodeMap_[id] = coordinate;

    std::shared_ptr<Node> node(new Node());
    node->id = id;
    node->coordinate = coordinate;
    utymap::utils::setTags(stringTable_, *node, tags);

    if (functor_(*node))
        statistics.nodes++;
    else
        statistics.skipNodes++;
}

void OsmDataVisitor::visitWay(std::uint64_t id, std::vector<std::uint64_t>& nodeIds, utymap::formats::Tags& tags)
{
    std::vector<GeoCoordinate> coordinates;
    coordinates.reserve(nodeIds.size());
    for (auto nodeId : nodeIds) {
        coordinates.push_back(nodeMap_[nodeId]);
    }

    if (coordinates.size() > 2 && isArea(tags)) {
        coordinates.pop_back();
        std::shared_ptr<Area> area(new Area());
        area->id = id;
        if (!utymap::utils::isClockwise(coordinates)) {
            std::reverse(coordinates.begin(), coordinates.end());
        }
        area->coordinates = std::move(coordinates);
        utymap::utils::setTags(stringTable_, *area, tags);
        areaMap_[id] = area;
        if (functor_(*area))
            statistics.areas++;
        return;
    } 

    std::shared_ptr<Way> way(new Way());
    way->id = id;
    way->coordinates = std::move(coordinates);
    utymap::utils::setTags(stringTable_, *way, tags);
    wayMap_[id] = way;
    if (functor_(*way))
        statistics.ways++;
    else
        statistics.skipWays++;
}

void OsmDataVisitor::visitRelation(std::uint64_t id, RelationMembers& members, utymap::formats::Tags& tags)
{
    if (hasTag("type", "multipolygon", tags)) {
        MultipolygonProcessor processor(id, members, tags, stringTable_, areaMap_, wayMap_);
        Relation relation = processor.process();
        if (!functor_(relation)) {
            statistics.skipRelations++;
            return;
        }
    }
    else {
        std::vector<std::shared_ptr<Element>> elements;
        elements.reserve(members.size());
        for (const auto& member : members) {
            if (member.type == "node") {
                std::shared_ptr<Node> node(new Node());
                node->coordinate = nodeMap_[member.refId];
                elements.push_back(node);
            }
            else if (member.type == "way") {
                auto areaPair = areaMap_.find(member.refId);
                if (areaPair != areaMap_.end()){
                    elements.push_back(areaPair->second);
                    continue;
                }
                auto wayPair = wayMap_.find(member.refId);
                if (wayPair != wayMap_.end())
                    elements.push_back(wayPair->second);
            }
            else {
                // not supported so far.
                statistics.skipRelations++;
                return;
            }
        }
    }

    statistics.relations++;
}

bool OsmDataVisitor::isArea(const utymap::formats::Tags& tags) const
{
    for (const auto& tag : tags) {
        if (AreaKeys.find(tag.key) != AreaKeys.end() &&
            FalseKeys.find(tag.value) == FalseKeys.end())
            return true;
    }

    return false;
}

bool OsmDataVisitor::hasTag(const std::string& key, const std::string& value, const utymap::formats::Tags& tags) const
{
    for (const auto& tag : tags) {
        if (tag.key == key && tag.value == value)
            return true;
    }
    return false;
}

OsmDataVisitor::OsmDataVisitor(StringTable& stringTable, std::function<bool(utymap::entities::Element&)> functor) :
    stringTable_(stringTable), functor_(functor)
{
}
