/*---------------------------------------------------------------
 * 功能：基于boost的正则表达式API封装
 * 作者：tang
 *---------------------------------------------------------------*/
#ifndef REGEX_HELPER_H_INCLUDE
#define REGEX_HELPER_H_INCLUDE
#include<string>
#include<vector>

#define DLL_REGEX_PUBLIC   __attribute__ ((visibility("default")))
#define REGEX_API extern "C" DLL_REGEX_PUBLIC

/*
 * 是否包含匹配字符串
 * @param: input 输入字符串
 * @param：pattern 正则表达式
 */
REGEX_API bool IsMatch(const char* input,const char* pattern);
/*
 * 获取首个匹配字符串或其字串
 * @param: input 输入字符串
 * @param：pattern 正则表达式
 * @param：group 子捕获组
 */
REGEX_API std::string MatchByGroup(const char* input,const char* pattern,int group = 0);
/*
 * 获取首个匹配字符串所有捕获组
 * @param: input 输入字符串
 * @param：pattern 正则表达式
 * @param: results 输出的字符串数组
 */
REGEX_API int MatchAllGroup(const char* input,const char* pattern,std::vector<std::string>& results);
/*
 * 匹配字符串数目
 * @param: input 输入字符串
 * @param：pattern 正则表达式
 */
REGEX_API int MatchesCount(const char* input,const char* pattern);
/*
 * 输出所有匹配字符串或其捕获组
 * @param: input 输入字符串
 * @param：pattern 正则表达式
 * @param: results 输出的字符串数组
 * @param：group 捕获组
 */
REGEX_API int MatchesAll(const char* input,const char* pattern,std::vector<std::string>& results,int group = 0);
/*
 * 替换首个匹配字符串
 * @param: input 输入字符串
 * @param：pattern 正则表达式
 * @param：repValue 被替换值，可以是捕获组的组合
 */
REGEX_API std::string ReplaceFirst(const char* input,const char* pattern,const char* repValue);
/*
 * 替换所有匹配字符串
 * @param: input 输入字符串
 * @param：pattern 正则表达式
 * @param：repValue 被替换值，可以是捕获组的组合
 */
REGEX_API std::string ReplaceAll(const char* input,const char* pattern,const char* repValue);
/*
 * 分割字符串并输出结果
 * @param: input 输入字符串
 * @param：pattern 正则表达式
 * @param: results 输出的字符串数组
 */
REGEX_API int Split(const char* input,const char* pattern,std::vector<std::string>& results);
/*
 * 分割字符串并根据捕获组输出
 * @param: input 输入字符串
 * @param：pattern 正则表达式
 * @param：subs 捕获组
 * @param: results 输出的字符串数组
 */
REGEX_API int SplitByGroup(const char* input,const char* pattern,std::vector<int>& subs,std::vector<std::string>& results);

#endif // REGEX_HELPER_H_INCLUDE
