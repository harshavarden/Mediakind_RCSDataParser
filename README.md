# Mediakind_RCSDataParser
(Project implemented in Mediakind as Trainee Developement Engineer) 
parser program for XML and json from Remote Configuration System (RCS)

****As to abide with company's policies, i have only uploaded the code developed by me, instead of uploading whole code base****

The Feature Request - was to develop the Parser for XML based Remote Configuration System (RCS) which was used to hold the config data. RCS was xml code base used to hold config data to enable or disable the feature remotely, change the versions of APIs etc.

The Parser program was hence named as RCSDataParser.cpp, i used map{key, value} to hold XML data. the XML data consisted of keys of string type and value can be string or int or boolean type. the map implemented to finally store these data was of type ***map<string key, boost::variant<bool, string, int>>***. 
the boost::variant<bool, int, string> allows the value type for map can be any one of these to be associated with each key.

the singleton pattern was implemented to allow same instance of RCSDataParser to used for all processes. the InitializeRCSDataPArser() would be called at first in any controller files in order to initialize the parser. the keys has to be of enum type RcsKeyData and hence to be included in RcsDataKeysNames[] char array.
the InitializeRCSDataParser() would use read_json() to read json and read_xml() to read xml data. the boost::propert_tree is used to hold the values retrived from read_json() and read_xml, which is then translated into rcs_map{}. the BOOST_FOREACH() is used to iterate over each key value pair in property_tree ptree and and add those values into rcs_map{}.
