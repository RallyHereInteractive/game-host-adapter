#pragma once
#include <fstream>
#include "boost/json.hpp"

struct Profile
{
    std::string environment_id{};
    std::string base_url{};
    std::string client_id{};
    std::string client_secret{};
    std::string username{};
    std::string password{};
};

inline void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, Profile const& p)
{
    jv = boost::json::object{
        {"environment_id", p.environment_id},
        {"base_url", p.base_url},
        {"client_id", p.client_id},
        {"client_secret", p.client_secret},
        {"username", p.username},
        {"password", p.password},
    };
}

inline Profile tag_invoke(boost::json::value_to_tag<Profile>, boost::json::value const& jv)
{
    Profile p{};
    p.environment_id = jv.at("EnvironmentId").as_string().c_str();
    p.base_url = jv.at("BaseURL").as_string().c_str();
    auto client_id_src = jv.as_object().if_contains("ClientId");
    if (client_id_src && client_id_src->is_string())
    {
        p.client_id = client_id_src->as_string().c_str();
    }
    auto client_secret_src = jv.as_object().if_contains("ClientSecret");
    if (client_secret_src && client_secret_src->is_string())
    {
        p.client_secret = client_secret_src->as_string().c_str();
    }
    auto username_src = jv.as_object().if_contains("Username");
    if (username_src && username_src->is_string())
    {
        p.username = username_src->as_string().c_str();
    }
    auto password_src = jv.as_object().if_contains("Password");
    if (password_src && password_src->is_string())
    {
        p.password = password_src->as_string().c_str();
    }
    return p;
}

struct Profiles
{
    std::vector<Profile> profiles{};
};

inline void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, Profiles const& p)
{
    boost::json::array arr(jv.storage());
    for (auto& item : p.profiles)
    {
        arr.emplace_back(boost::json::value_from(item));
    }
    jv = boost::json::object{
        {"profiles", arr},
    };
}

inline Profiles tag_invoke(boost::json::value_to_tag<Profiles>, boost::json::value const& jv)
{
    Profiles p{};
    auto arr = jv.at("profiles").as_array();
    for (auto& item : arr)
    {
        p.profiles.push_back(boost::json::value_to<Profile>(item));
    }
    return p;
}

inline Profiles load_profiles(std::string const& path)
{
    std::ifstream file(path.c_str());
    boost::json::value jv;
    file >> jv;
    return boost::json::value_to<Profiles>(jv);
}