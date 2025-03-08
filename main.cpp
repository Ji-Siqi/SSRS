#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cjson/cJSON.h>

struct Student {
    std::string number;
    int score;
};

int main() {
    // 读取输入文件
    std::ifstream inFile("in.json");
    if (!inFile) {
        std::cerr << "无法打开输入文件" << std::endl;
        return 1;
    }
    
    std::string jsonStr((std::istreambuf_iterator<char>(inFile)), 
                       std::istreambuf_iterator<char>());
    inFile.close();

    // 解析JSON
    cJSON* root = cJSON_Parse(jsonStr.c_str());
    if (!root) {
        std::cerr << "JSON解析失败" << std::endl;
        return 1;
    }

    std::vector<Student> students;
    cJSON* item = nullptr;
    cJSON_ArrayForEach(item, root) {
        students.push_back({
            cJSON_GetObjectItem(item, "number")->valuestring,
            cJSON_GetObjectItem(item, "score")->valueint
        });
    }
    cJSON_Delete(root);

    // 按分数降序排序
    std::sort(students.begin(), students.end(), 
              [](const Student& a, const Student& b) { return a.score > b.score; });

    // 计算排名
    cJSON* output = cJSON_CreateArray();
    int rank = 1;
    for (size_t i = 0; i < students.size(); ++i) {
        if (i > 0 && students[i].score != students[i-1].score) {
            rank = i + 1;
        }
        
        cJSON* obj = cJSON_CreateObject();
        cJSON_AddStringToObject(obj, "number", students[i].number.c_str());
        cJSON_AddNumberToObject(obj, "score", students[i].score);
        cJSON_AddNumberToObject(obj, "rank", rank);
        cJSON_AddItemToArray(output, obj);
    }

    // 写入输出文件
    char* json = cJSON_Print(output);
    std::ofstream outFile("out.json");
    outFile << json;
    outFile.close();

    cJSON_Delete(output);
    free(json);
    
    std::cout << "处理完成，结果已保存到out.json" << std::endl;
    return 0;
}