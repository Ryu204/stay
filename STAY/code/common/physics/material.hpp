// #pragma once

// #include <box2d/box2d.h>

// #include "../type/serializable.hpp"

// namespace stay
// {
//     namespace phys
//     {
//         class Collider;
//         struct Material : Serializable
//         {
//                 // Unit: scalar
//                 float friction{0.2F};
//                 // Unit: kg/m^2
//                 float density{0.F};
//                 Json::Value toJSONObject() const override
//                 {
//                     Json::Value res;
//                     res["friction"] = friction;
//                     res["density"]  = density;
//                 }
//                 bool fetch(const Json::Value& data) override
//                 {
//                     if (data.isObject() && data["friction"].isNumeric() && data["density"].isNumeric())
//                     {
//                         friction = data["friction"].asFloat();
//                         density = data["density"].asFloat();
//                         return true;
//                     }
//                     return false;
//                 }
//             private:
//                 friend class Collider;
//                 // Fetch the data into `fixture`
//                 // @note This function will not change friction of existing contacts
//                 void updateB2dFixture(b2Fixture* fixture) const
//                 {
//                     fixture->SetFriction(friction);
//                     fixture->SetDensity(density);
//                     fixture->GetBody()->ResetMassData();
//                 }
//         };
//     } // namespace phys
// } // namespace stay
