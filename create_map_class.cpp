#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <iostream>

using namespace boost::interprocess;


//���ͺ�allocator�Ķ��壬ʹ�ù����ڴ�ʱ��Ҫʹ��boost::interprocess��
//����ʵ�ֵ�����������ʹ�ñ�׼����

typedef managed_shared_memory::segment_manager                       segment_manager_t;
typedef allocator<void, segment_manager_t>                           void_allocator;

/*-- �Զ���string�������� --*/
typedef allocator<char, segment_manager_t>                           char_allocator;
typedef basic_string<char, std::char_traits<char>, char_allocator>   char_string;

/*-- �Զ���vector�������� --*/
typedef allocator<int, segment_manager_t>                            int_allocator;
typedef vector<int, int_allocator>                                   int_vector;
typedef vector<int, int_allocator>::iterator                         int_vector_it;

/*-- �Զ���map<int, float>�������� --*/
typedef std::pair<const int, float> int_float_value_type;
typedef allocator<int_float_value_type, segment_manager_t> int_float_map_allocator;
typedef map<int, float, std::less<int>, int_float_map_allocator>  int_float_map;
typedef map<int, float, std::less<int>, int_float_map_allocator>::iterator int_float_map_it;

/*-- ����һ������map<int ,float>��class --*/
class class_with_map
{
	int id;
	int_float_map class_map;
	int_vector    test_vector;

public:
	class_with_map(int id, const void_allocator& void_alloc) :id(id), class_map(void_alloc),test_vector(void_alloc) {}

	virtual ~class_with_map(){}

	int get_id() { return id; }
	void set_id(int temp_id) { id = temp_id; }

	void c_m_insert(int a, float b)
	{
		int_float_value_type int_float_value(a, b);
		class_map.insert(int_float_value);
	}

	void vector_push_back(int tmp)
	{
		test_vector.push_back(tmp);
	}

	void printf_vector()
	{
		int_vector_it it;
		for (it = test_vector.begin(); it != test_vector.end(); it++)
		{
			std::cout << "vector_number:" << *it << std::endl;
		}
	
	}

	void printf_map()
	{
		int_float_map_it it;
		for (it = class_map.begin(); it != class_map.end(); it++)
		{
			std::cout << "first = " << it->first << std::endl;
			std::cout << "second = " << it->second << std::endl;
		}
	}

};

/*-- �Զ���map<int, class>�������� --*/
typedef std::pair<const int, class_with_map> int_class_type;
typedef allocator<int_class_type, segment_manager_t> int_class_allocator;
typedef map<int, class_with_map, std::less<int>, int_class_allocator> int_class_map;
typedef map<int, class_with_map, std::less<int>, int_class_allocator>::iterator int_class_map_it;

int main()
{
	shared_memory_object::remove("MySharedMemory");

	managed_shared_memory segment(open_or_create, "MySharedMemory", 102400);

	void_allocator alloc_inst(segment.get_segment_manager());

	int_class_map* class_map = segment.construct<int_class_map>("class_map")(std::less<int>(), alloc_inst);

	class_with_map c_w_m(14, alloc_inst);

	int_class_type int_class_value(20, c_w_m);

	class_map->insert(int_class_value);

	//class_map->second->second->c_m_insert(20, 21.123);

	int_class_map_it it;

	it = class_map->find(20);
	
	it->second.c_m_insert(32, 42.515);

	it->second.vector_push_back(32);

	it->second.vector_push_back(234);

	it->second.vector_push_back(23);


	return 0;
}