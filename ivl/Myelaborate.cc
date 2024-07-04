
# include  "pform.h"
# include  "PClass.h"
# include  "PEvent.h"
# include  "PGenerate.h"
# include  "PPackage.h"
# include  "PScope.h"
# include  "PSpec.h"
# include  "parse_api.h"
# include  "Mynetlist.h"
#include "utils.h"

using namespace std;

std::map<std::string, MYWIRE*> sub_exp;

std::map<std::string, int> id2num_store;

// idת���
int id2num(std::string id)
{
	if (id2num_store.count(id) > 0)
		return id2num_store[id];
	return id2num_store[id] = id2num_store.size() + 1;
}

//// vertor������Ԫ��
//void changeCell(vector <MYCELL> input, IdString str, IdString name)
//{
//	cout << "enter change cell" << endl;
//	// ����
//	for (auto iter :input ){
//		// �ҵ�һ��cell�к��и��������
//		if (iter.cell_conns.count(str) > 0) {
//			cout << "find one " << endl;
//			MYPORT port3 = { "Y",1 };
//			iter.cell_conns.insert({name,port3});
//
//			/*for (auto conn : iter.cell_conns) {
//				cout << "conn name" << conn.first << endl;
//				cout << "conn port" << conn.second.portname << endl;
//			}*/
//		}
//	}
//
//
//	//vector<int>::iterator iter = std::find(input.begin(), input.end(), number);//���ص���һ��������ָ��
//	//if (iter == input.end())
//	//{
//	//	return -1;
//	//}
//	//else {
//	//	return std::distance(input.begin(), iter);
//	//}
//}


int get_fileline()
{
	return 0;
}
MyScope::MyScope(void)
{

}

MyScope::~MyScope(void)
{

}

MyModule::MyModule(void)
{

}

MyModule::~MyModule(void)
{

}
MyDesign::MyDesign(void)
{

}

MyDesign::~MyDesign(void)
{

}

string elaborate_ident(PEIdent* ident)
{
	pform_name_t ls = ident->path();
	list<name_component_t>::iterator s = ls.begin();
	return (*s).name.str();       
}

string  MyModule::elaborate_expression(PExpr* expr)
{	
	cout << "<ela_exp>" << endl;
	string exp;
	if (PEIdent* ident = dynamic_cast<PEIdent*>(expr)) 
	{
		cout << "Enter ident branch" << endl;
		exp =  elaborate_ident(ident);
		cout << "Final exp is " << exp << endl;
		cout << "</ela_exp>" << endl;
		return exp;
	}
	else if (PEBinary* binary = dynamic_cast<PEBinary*>(expr)) {

		cout << "Enter binary branch" << endl;
		PExpr* left = (PExpr*)binary->get_left();
		string str_left = elaborate_expression(left);
		cout << "str_left is " << str_left << endl;

		char Op = binary->get_op();
		string sch(1, Op);

		PEIdent* right = (PEIdent*)binary->get_right();
		string str_right = elaborate_expression(right);
		cout << "str_right is " << str_right << endl;

		string strname = str_left + sch + str_right;
		cout << "strname is " << strname << endl;



		// ÿ�������ظ���strname���������һ�����򣬶������һ��temp��wire
		if (m_mids.count(strname) > 0 ){
			// ���������ͬ�ļ�
			cout << "meet the same" << endl;
			if (m_mids[strname].number == 1) {
				// ���
				
				cout << "<wire>" << endl;
				MYWIRE tmp;
				cout << "temp" << endl;
				tmp.name = (IdString)stringf("temp%d",id2num(strname));
				cout << "wire name is " << tmp.name << endl;
				tmp.porttype = NOT_A_PORT2;
				cout << "wire portype is " << tmp.porttype << endl;
				//tmp.port_input = 1;
				//tmp.port_output = 1;
				m_wires.push_back(tmp);
				cout << "</wire>" << endl;
				m_mids[strname].trans_name = tmp.name;
				m_mids[strname].number++;

			}

			for (int i = 0; i < m_cells.size(); ++i) {
				// �ҵ�һ��cell�к��и��������
				if (m_cells[i].cell_conns.count(strname) > 0) {
					cout << "find one, name is "<<m_cells[i].name << endl;
					MYPORT t;
					// ����ֵ
					t = m_cells[i].cell_conns[strname];
					// ɾ��Ԫ��
					m_cells[i].cell_conns.erase(strname);
					// ����Ԫ��
					m_cells[i].cell_conns.insert({ m_mids[strname].trans_name,t });


				}
			}
		}
		else {
			m_mids[strname].number = 1;
			// һ��cell����һ������
			MYCELL cell;
			cell.name = "$" + to_string(m_cells.size() + 1);
			cout << "cell " << cell.name << " was inserted" << endl;
			cell.type = "$" + sch;
			MYPORT port1 = { "A",0 };
			MYPORT port2 = { "B",0 };
			MYPORT port3 = { "Y",1 };
			if (m_mids.count(str_left) > 0 && m_mids[str_left].number > 1) {
				cout << "left meet the same" << endl;
				cout << "real name is " << str_left << endl;
				cout << "name is " << m_mids[str_left].trans_name << endl;
				cout << "num is " << m_mids[str_left].number << endl;
				cell.cell_conns.insert({ m_mids[str_left].trans_name, port1 });
			}
			//else if (m_mids.count(str_left) > 0 && m_mids[str_left].number == 1) {
			//	for (int i = 0; i < m_cells.size(); ++i) {
			//		// �ҵ�һ��cell�к��и��������
			//		if (m_cells[i].cell_conns.count(str_left) > 0) {
			//			cout << "find one, name is " << m_cells[i].name << endl;
			//			MYPORT t;
			//			// ����ֵ
			//			t = m_cells[i].cell_conns[str_left];
			//			// ɾ��Ԫ��
			//			m_cells[i].cell_conns.erase(str_left);
			//			// ����Ԫ��
			//			m_cells[i].cell_conns.insert({ m_mids[str_left].trans_name,t });
			//		}
			//	}
			//}
			else 
				cell.cell_conns.insert({ str_left, port1 });
			if (m_mids.count(str_right) > 0 && m_mids[str_right].number > 1) {
				cout << "left meet the same" << endl;
				cout << "real name is " << str_right << endl;
				cout << "name is " << m_mids[str_right].trans_name << endl;
				cout << "num is " << m_mids[str_right].number << endl;
				cell.cell_conns.insert({ m_mids[str_right].trans_name, port2 });
			}
			else
				cell.cell_conns.insert({ str_right, port2 });
			cell.cell_conns.insert({ strname, port3 });
			m_cells.push_back(cell);

		

			cout << "</ela_exp>" << endl;
		}

		//if (m_mids.count(str_left) > 0) {
		//	for (int i = 0; i < m_cells.size(); ++i) {
		//		// �ҵ�һ��cell�к��и��������
		//		if (m_cells[i].cell_conns.count(str_left) > 0) {
		//			cout << "find one " << endl;
		//			MYPORT t;
		//			// ����ֵ
		//			t = m_cells[i].cell_conns[str_left];
		//			// ɾ��Ԫ��
		//			m_cells[i].cell_conns.erase(str_left);
		//			// ����Ԫ��
		//			m_cells[i].cell_conns.insert({ m_mids[str_left].trans_name,t });


		//		}
		//	}
		//}

		//if (m_mids.count(str_right) > 0) {
		//	for (int i = 0; i < m_cells.size(); ++i) {
		//		// �ҵ�һ��cell�к��и��������
		//		if (m_cells[i].cell_conns.count(str_right) > 0) {
		//			cout << "find one " << endl;
		//			MYPORT t;
		//			// ����ֵ
		//			t = m_cells[i].cell_conns[str_right];
		//			// ɾ��Ԫ��
		//			m_cells[i].cell_conns.erase(str_right);
		//			// ����Ԫ��
		//			m_cells[i].cell_conns.insert({ m_mids[str_right].trans_name,t });


		//		}
		//	}
		//}



		return strname;
	}

}


bool MyModule::elaborate_gate(Module* rmod)
{	
	cout << " <elaborate_gate>" << endl;
	const list<PGate*>& gl = rmod->get_gates();

	for (list<PGate*>::const_iterator gt = gl.begin()
		; gt != gl.end(); ++gt) {
		PGate* cur = *gt;
		cout << "<each_gate>" << endl;
		///suppose expression: q = a + b + c
		string str_finalout = elaborate_expression(cur->pin(0));     ///q
		cout << "str_finalout is " << str_finalout << endl;
		string str_allright = elaborate_expression(cur->pin(1));
		cout << "str_allright is " << str_allright << endl;

		m_cells.back().cell_conns.erase(str_allright);

		MYPORT port3 = { "Y",1 };

		m_cells.back().cell_conns.insert({ str_finalout, port3 });
		cout << "</each_gate>" << endl;
	}
	cout << "</elaborate_gate>" << endl;
	return true;
}
bool MyModule::elaborate_sig(Module* rmod)
{
		cout << "<ela_sig>" << endl;
		bool flag = true;
        //get wire
		for (map<perm_string, PWire*>::const_iterator wt = rmod->wires.begin()
			; wt != rmod->wires.end(); ++wt) {
			cout << "<wire>" << endl;
			PWire* cur = (*wt).second;
			MYWIRE tmp;
			tmp.name = cur->basename();
			cout << "wire name is " << tmp.name << endl;
			tmp.porttype = (enum MY_PORT_TYPE) cur->get_port_type();
			cout << "wire portype is " << tmp.porttype << endl;
			//tmp.port_input = 1;
			//tmp.port_output = 1;
			m_wires.push_back(tmp);
			cout << "</wire>" << endl;
		}
		///get port
		for (unsigned idx = 0; idx < rmod->ports.size(); idx += 1) {
			cout << "<port>" << endl;
			Module::port_t* pp = rmod->ports[idx];
			MYPORT tmp;
			const char* namestr = pp->name.str();
			cout << "port name is "<< namestr << endl;

			tmp.portname += *namestr;
			cout << "port name is " << tmp.portname << endl;
			tmp.inout_type = 0;
			cout << "port in_out_type is " << tmp.inout_type << endl;
			m_ports.push_back(tmp);
			cout << "</port>" << endl;
		}
		cout << "</ela_sig>" << endl;

	return true;
}

MyDesign* My_elaborate(list<perm_string>roots)
{

      MyDesign*des = new MyDesign;

	  for (list<perm_string>::const_iterator root = roots.begin()
		  ; root != roots.end(); ++root) {

		  map<perm_string, Module*>::const_iterator mod = pform_modules.find(*root);

		  Module* rmod = (*mod).second;

		  //des->scope.set_num_ports( rmod->port_count() );

		  des->scope.elaborate_sig(rmod);
		  des->scope.elaborate_gate(rmod);

	  }
      return des;
}

int My_show_netlist(MyDesign*des)
{
	cout << "<show_netlist>" << endl;
	std::string module_name = "add0";

	// ��������.dot�ļ�
	FILE* dotfile = fopen("output\\test1.dot", "w");
	if (dotfile == nullptr) {
		printf("failed to open the file.");
		return -1;
	}

	// д��.dot�ļ���ͷ����Ϣ
	fprintf(dotfile, "digraph \"%s\" {\n", module_name.c_str());
	fprintf(dotfile, "label=\"%s\";\n", module_name.c_str());
	fprintf(dotfile, "rankdir=\"LR\";\n");
	fprintf(dotfile, "remincross=true;\n");

	////////////////get wire()/////////////////
	std::set<std::string> all_sources, all_sinks;

	cout << "<get_wire>" << endl;
	for (auto wire : des->scope.m_wires) {
		const char* shape = "diamond";
		if (wire.porttype == PINPUT2 || wire.porttype == POUTPUT2 || wire.porttype == PINOUT2) {
			shape = "octagon";
		}
		fprintf(dotfile, "n%d [ shape=%s, label=\"%s\"];\n", id2num(wire.name), shape, wire.name.c_str());
		cout << "n is " << id2num(wire.name)<< endl << "shape is " << shape << endl << "label is " << wire.name.c_str() << endl;
		if (wire.porttype == PINPUT2 || wire.porttype == PINOUT2)
		{
			cout << id2num(wire.name) << " is source" << endl;
			all_sources.insert(stringf("n%d", id2num(wire.name)));
		}

		if (wire.porttype == POUTPUT2 || wire.porttype == PINOUT2)
		{
			cout << id2num(wire.name)<< " is sink" << endl;
			all_sinks.insert(stringf("n%d", id2num(wire.name)));
		}
	}
	cout << "</get_wire>" << endl;

	cout << "<get_cell>" << endl;
	for (auto cell : des->scope.m_cells) {
		// ��ÿ��cellʵ���е�portʵ������,ÿ��cell�е�conn����¼������˭�����룬�����������ȥ
		// ��һ������wire�ģ��ڶ�������port��port�е�inout_type��������¼���������
		cout <<"<" << cell.name << "_cell>" << endl;
		std::vector<std::string> in_ports, out_ports;
		std::vector<std::string> in_label_pieces, out_label_pieces;
		// ����ÿ������
		for (auto conn : cell.cell_conns) {
			////getports()///////
			cout << conn.second.portname << endl;
			if (conn.second.inout_type == 0){
				// ���������ģ��Ǿͽ�����ӵ����������
				cout << "is 0" << endl;
				in_ports.push_back(conn.second.portname);
			}
			else {
				// ���������ģ��ͽ�����ӵ����������
				cout << "is not 0" << endl;
				out_ports.push_back(conn.second.portname);
			}
		}
		for (auto& p : in_ports) {
			// ����ÿ��������У���������Լ�����
			cout << "in label " << stringf("p%d %s", id2num(p), p.c_str()) << endl;
			in_label_pieces.push_back(stringf("<p%d> %s", id2num(p), p.c_str()));
		}

		for (auto& p : out_ports) {
			// ����ÿ��������У���������Լ�����
			cout << "out label " << stringf("p%d %s", id2num(p), p.c_str()) << endl;
			out_label_pieces.push_back(stringf("<p%d> %s", id2num(p), p.c_str()));
		}

		// ��ÿ���ַ���ʹ��"|"����
		IdString in_label = join_label_pieces(in_label_pieces);
		IdString out_label = join_label_pieces(out_label_pieces);

		std::string label_string = stringf("{{%s}|%s\\n%s|{%s}}", in_label.c_str(),
			cell.name.c_str(), cell.type.c_str(), out_label.c_str());
		cout << "label_str " << label_string.c_str() << endl;

		fprintf(dotfile, "c%d [ shape=record, label=\"%s\", ];\n",
			id2num(cell.name), label_string.c_str());

		//////////////////get connetction()//////////////////////////
		cout << "<get_connect>" << endl;
		// ����cell_conns�е�ÿһ��Ԫ�أ�����д������Ϣ
		for (auto conn : cell.cell_conns) {
			// ��ȡport����
			std::string portname = stringf("c%d:p%d", id2num(cell.name), id2num(conn.second.portname));
			cout << "protname " << portname << endl;
			// ��ȡwire����
			std::string net = stringf("n%d", id2num(conn.first));
			cout << "conn first " << conn.first << endl;
			cout << "net " << net << endl;
			if (conn.second.inout_type == 0) //in port �����
			{
				fprintf(dotfile, "%s:e -> %s:w;\n", net.c_str(), portname.c_str());
			}
			else {
				fprintf(dotfile, "%s:e -> %s:w;\n", portname.c_str(), net.c_str());
			}
				

		}
		cout << "</get_connect>" << endl;
		cout << "</" << cell.name << "_cell>" << endl;

	}
	cout << "</get_cell>" << endl;

	// д��.dot�ļ���β����Ϣ
	fprintf(dotfile, "}\n");
	cout << "</show_netlist>" << endl;

	// �ر�.dot�ļ�
	fclose(dotfile);

	printf("Dot file generated successfully.");

	return 0;
}
