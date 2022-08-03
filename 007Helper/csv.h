/* csv文件解析
 * author:chengf
 * date:2018-12-31
 */

#define TYPE_NIL     0
#define TYPE_INT     1
#define TYPE_DOUBLE  2
#define TYPE_STRING  3
#define TYPE_BOOL    4

#ifdef __cplusplus
extern "C"
{
#endif

struct csv_value {
	int type;

	int intvalue;
	double doublevalue;
	char* stringvalue;
};

struct csv_value_list {
	struct csv_value value;
	struct csv_value_list* next;
};

struct csv_data {
	struct csv_data* next;//下一行
	struct csv_value_list* value_list;//value list
};

//获取出错位置文本
extern const char* Csv_GetErrorPtr();

//加载Csv文件为C结构
extern struct csv_data* Csv_Load(const char* filename);

extern struct csv_data* Csv_LoadEx(char* Buff);

//释放C结构
extern void Csv_Delete(struct csv_data* data);

//解析文本为C结构
extern struct csv_data* Csv_Parse(const char* strdata);

//输出c结构
extern void Csv_PrintData(struct csv_data* data);

//输出c结构到文件
extern int Csv_PrintToFile(struct csv_data* data, const char* filename);

#ifdef __cplusplus
}
#endif