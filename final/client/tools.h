#ifndef TOOLS_H
#define TOOLS_H
class tool{
private:
public:
    int  splitString(const string & strSrc, const std::string& strDelims, vector<string>& strDest)
    {
    typedef std::string::size_type ST;
    string delims = strDelims;
    std::string STR;
    if(delims.empty()) delims = "\n\r";


    ST pos=0, LEN = strSrc.size();
    while(pos < LEN ){
    STR="";
    while( (delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN) ) ++pos;
    if(pos==LEN) return strDest.size();
    while( (delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN) ) STR += strSrc[pos++];
    if( ! STR.empty() ) strDest.push_back(STR);
    }
    return strDest.size();
    }

};


#endif // TOOLS_H
