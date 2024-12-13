/*
 * RCSDataParser.cpp
 *
 * Created on: Dec 20, 2023
 *
 * Copyright (c) Mediakind, Inc.
 */

#include "RCSDataParser.hpp"
#include <boost/propert_tree/ptree.hpp>
#include <boost/serialization/variant.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::property_tree;

RCSDataParser *RCSDataParser::m_rcsptr = NULL;

RCSDataParser::RCSDataParser(){
}

RCSDataParser::~RCSDataParser{
}

//to insert boolean data
void insertBoolDataToRCSMap(const string& key, const string& value){
    RCSValue rcs_val = false;
    if(!key.empty() && (value == "true" || value == "false")){
        if(value == "true"){
            rcs_val = true;
        }
        m_rcsmap[key] = rcs_val;
    }
}

//to insert string data
void insertStringDataToRCSMap(const string& key, const string& value){
    if(!key.empty() && ! value.empty()){
        m_rcsmap[key] = value;
    }
}

//to insert int data
void insertIntDataToRCSMap(const string& key, const string& value){
    RCSValue rcs_val;
    if(!key.empty() && !value.empty()){
        try{
            rcs_val = boost::lexical_cast<int>(value);
            m_rcsmap[key] = rcs_val;
        }catch (boost::bad_lexical_cast &e){
            AZ_LOG(ERROR, ACC, "insertIntDataToRCSMap::"+string(e.what()+" for key : "+key+"and value "+value));
        }
    }
}

//read json and xml data in given file and add the data to rcs_map
void RCSDataParser::InitializeRCSDataParser(string &rcsConfigFile){
    ptree xml_pt, json_pt;
    try{
        read_json(rcsConfigFile, json_pt);
    }catch(std::exception &e){
        AZ_LOG(ERROR, ACC, "InitializeRCSDataParser::Malformed Json string:" + string(e.what()));  
        return;
    }

    if(!json_pt.empty()){
        try{
            string rcs_xml = "";
            rcs_xml = json_pt.get<std::string>("ConfigString");
        }catch(boost::property_tree::ptree_bad_path &e){
            AZ_LOG(ERROR, ACC, "InitializeRCSDataParser::Missing ConfigString");
            return;
        }

        if(!rcs_xml.empty()){
            stringstream rcsXmlStream(rcs_xml);
            try{
                read_xml(rcsXmlStream, xml_pt);
            }catch(std::exception &e){
                AZ_LOG(ERROR, ACC "InitializeRCSDataParser::Malformed XML String: " + string(e.what()));
                return;
            }
        }

        else{
            AZ_LOG(ERROR, ACC, "InitializeRCSDataParser::Empty Json String");
            return;
        }
    }

    BOOST_FOREACH(ptree::value_type const& v, xml_ptree.get_child("remoteConfig").get_child("config")){
        /*sample data 
        <remoteConfig>
            <config>
                <item key="setting1">true</item><!--bool_type--!>
                <item key="setting2">example_value</item><!--string_type--!>
                <item key="setting3">42</item><!--int_type--!>
            </config>
       </remoteConfig>*/
        string rcskey = v.second.get_child("<xmlattr>.key").data();
        string rcsvalue = v.second.data();
        if(!rcskey.empty() && !rcsvalue.empty()){
            if(booleanTypekey.find(rcskey) != booleanTypekey.end()){
                insertBoolDataToRCSMap(rcskey, rcsvalue);
            }
            else if(stringTypekey.find(rcskey) != stringTypekey.end()){
                insertStringDataToRCSMap(rcskey, rcsvalue);
            }
            else if{
                insertIntDataToRCSMap(rcskey, rcsvalue);
            }
        } 

    }
    //to print the available key value pairs into logs
    for(RCSMap::const_iterator it = m_rcsmap.begin(), it != m_rcsmap.end(), ++it){
           AZ_LOG(INFO, ACC, "RCS-DATA:: " + AZ_STR(it->first) + " = " + AZ(it->second));
    }

}

//perform singleton
RCSDataParser* RCSDataParser::getInstance(){
    if(m_rcsptr == NULL){
        m_rcsptr = new RCSDataParser;
    }
    return m_rcsptr;
}

//to retrive all type of data from rcs_map
bool RCSDataParser::getDataFromRCSMap(const string& key, const RCSValue& value){
    if(key.empty()){
        return false;
    }
    else {
        auto itr = m_rcsmap.find(key);
        if(! itr = m_rcsmap.end()){
            value = m_rcsmap[key];
            return true;
        }
        else {
            return false;
        }
    }
}

//to get boolean type value
void RCSDataParser::GetBoolRcsConfig(RcsKeyData rcskey, bool &value){
    RCSValue rcsBool_val;
    RCSDataParser* rcs_ptr = RCSDataParser::getInstance();
    if(rcs_ptr->gatDataFromRCSMap(RcsDataKeysNames[rcskey], rcsBool_val)){
        value = boost::get<int>(rcsBool_val);
    }
}

//to get string type value
void RCSDataParser::GetStringRcsConfig(RcsKeyData rcskey, string &value){
    RCSValue str_rcs_val;
    RCSDataParser* rcs_ptr = RCSDataParser::getInstance();
    if(rcs_ptr->getDataFromRCSMap(RcsDataKeysNames[rcskey], str_rcs_val)){
        value = boost::get<string>(str_rcs_val);
    }
}

// to get int type value
void RCSDataParser::GetIntRcsConfig(RcsKeyData rcskey, int &value){
    RCSValue rcs_int_val;
    RCSDataParser* rcs_ptr = RCSDataParser::getinstance();
    if(rcs_ptr->getDataFromRCSMap(RcsDataKeysNames[rcskey], rcs_int_val)){
        value = boost::get<int>(rcs_int_val);
    }
}

//to get ptree type value
void RCSDataParser::GetPtreeRcsConfig(RcsKeyData rcskey, ptree &pt_val){
    string json_str = "";
    GetStringRcsConfig(rcskey, json_str);
    if(!json_str.empty()){
        stringstream ss;
        ss << json_str;
        try{
            boost::property_tree::json_parser::read_json(ss, pt_val);
        }catch(std::exception &e){
            AZ_LOG(ERROR, ACC, "GetPtreeRcsConfig::Malformed JSON String "+string(e.what()));
            return;
        }
    }
}

//to set boolean type value to rcs_map
void RCSDataParser::setBoolRcsConfig(RcsKeyData key, const string &val){
    RCSDataParser* rcs_ptr = RCSDataParser::getInstance();
    rcs_ptr->insertBoolDataToRCSMap(RcsDataKeysNames[key], val); 
}

//to set string type value to rcs_map
void RCSDataParser::setStringRcsConfig(RcsKeyData key, const string &val){
    RCSDatParser* rcs_ptr = RCSDataParser::getInstance();
    rcs_ptr->insertStringDatToRCSMap(RcsKeyData[key], val);
}

//to set int type value to rcs_map
void RCSDataParser::setIntRcsConfig(RcskeyData key, const string &val){
    RCSDataParser* rcs_ptr = RCSDataParser::getInstance();
    rcs_ptr->insertIntDataToRCSMap(RcsKeyData[key], val);
}