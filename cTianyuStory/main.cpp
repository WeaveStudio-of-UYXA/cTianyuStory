//虽然不是很想承认，但不得不说Py和Qt互译多是件美事
//对于此项目，其实主要问题在于，Py本身是动态语言，写个小py文件再按个F5也没啥大不了的，要不了多长时间
//但是C++吧，这玩意你F5就编译了，编译时长，看编译器，还看你到底写了个啥。
//所以总的来说，编译长时和自解释长时你得二选一
//顺带一提，C++有宏不用可惜了，这就把Py显式函数写法全部换掉
//utf8编译
#pragma execution_character_set("utf-8")
#include <QApplication>
#include "test.h"

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	Part1 p1;
	return app.exec();
}