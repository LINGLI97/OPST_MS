
#include <iostream>
#include <string>
#include <sdsl/wavelet_trees.hpp>
#include <fstream>


//#include "Successor.h"
//#include "Predecessor.h"

#include "OPST_MS.h"

//#include "baseline.h"
#include "oracle.h"
using namespace std;
using namespace sdsl;
#include <filesystem>
//#define terminate_label 0
//#include "cmdline.h"




void readfile(const string &filename, std::vector<int> &normalVec){
    std::ifstream f(filename);
    if (!f) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }


    int c;
    while (f >> c) {
        normalVec.push_back(c);
    }
    f.close();

}
std::string get_filename(const std::string& path) {

    size_t last_slash_idx = path.find_last_of("/\\");
    if (last_slash_idx == std::string::npos) {
        return path;
    }
    return path.substr(last_slash_idx + 1);
}

std::string get_grandparent_directory(const std::string& path) {
    // 找到最后一个斜杠的位置
    size_t last_slash_idx = path.find_last_of("/\\");
    if (last_slash_idx == std::string::npos) {
        return "";  // 没有斜杠，没有上级目录
    }
    // 获取去掉最后一个斜杠后的子串，并再次寻找斜杠
    size_t second_last_slash_idx = path.substr(0, last_slash_idx).find_last_of("/\\");
    if (second_last_slash_idx == std::string::npos) {
        return "";  // 只有一个斜杠，没有上级的上级目录
    }
    return path.substr(0, second_last_slash_idx);  // 返回第二个最后斜杠之前的部分
}



// 输入: suffix tree built from input_vecA (假设为P)，以及 input_vecB (作为T)
// 输出: vector<int> ms_table，其中 ms[i] 是从 T[i..] 与 P 的最长匹配长度

void compute_matching_statistics(OPST_MS& OP, oracle &B_oracle, vector<int> &ms_table) {
    auto root = OP.root;
    stNode* v = root;      // 当前匹配的节点
    int matched = 0;             // 当前匹配的长度

    int i = 0;
    while (i < ms_table.size()) {
        stNode* u = v;

        // 从当前节点继续匹配 T[i + l..] 对应 P 中的路径
        while (i +  matched < ms_table.size()) {
            int j = 0;
            int next_char = B_oracle.LastCodeInt(i , i + matched);
            stNode* child = u->getChild(next_char);
            if (!child) break;
            j++; // the match of length is at least 1

            // 当前边的长度 = child->depth - u->depth
            int edge_len = child->depth - u->depth;


            cout<<"--------------------------------------------------------------------------"<<endl;
            while (j < edge_len && i + matched + j < ms_table.size()) {

                int T_char = B_oracle.LastCodeInt(i, i +  matched + j);
                int P_char = OP.LastCodeInt(child->start , child->start + matched + j);

                if (T_char != P_char) break;
                j++;

                cout<<"-------------------------"<<endl;

            }

            matched += j;

            if (j < edge_len) break;  // 匹配未走完整条边，终止
            u = child; // 边匹配完，进入 child 节点
        }

        ms_table[i] = matched;

        // 设置下一轮的起始节点和匹配长度
        if (matched == 0 || u == root) {
            v = root;
            matched = 0;
        } else if (u->slink) {
            v = u->slink;
            matched = matched - 1;
        } else {
            while (u->slink == NULL){
                u = u->parent;
            }
            v = u->slink;
            matched = v->depth;
        }

        i++;
    }
}


//
//        int d =0;
//        bool match = true;
//        while ( match )
//        {
//            v = u->getChild( B_oracle.LastCodeInt(q, q+d) );
//            if ( v == NULL )
//            {
//                match = false;
//                break;
//            }
//            INT i, j;
//            for ( i = d, j = d; i < B_oracle.n && j < v->depth ; i++, j++ )
//            {
////            cout<<"P["<<i<<"]"<<P[i]<<endl;
////            cout<<"T["<<v->start+j<<"]"<<this->T[v->start+j]<<endl;
//
//                if ( B_oracle.LastCodeInt(q, q+i) != OP.LastCodeInt(v->start,v->start+j) )
//                {
//                    match = false;
//                    break;
//                }
//            }
//            d = v->depth;
//            u = v;
//            if ( d >= B_oracle.n - q )
//            {
//
//
//            }
//        }
//        q++;
//    }
//
//
//
//
//
//
//
//    while (i < T.size()) {
////        int l = match_length;
//        auto node = current_node;
////        int edge_pos = 0;
//
//        while (l < T.size() - i) {
//            int next_char = T[i + l];
//            if (node->getChild(OP.LastCodeInt(i, i + l))){
//                auto edge = OP.get_edge(node, next_char);
//                int j = 0;
//                while (j < edge.length && l + j < T.size() - i && OP.get_char(edge, j) == T[i + l + j]) {
//                    j++;
//                }
//                l += j;
//                if (j < edge.length) break; // mismatch
//                node = edge.child;
//            } else {
//                break;
//            }
//        }
//
//        ms_table[i] = l;
//
//        // 准备下一轮，利用 suffix link 来加速
//        if (l == 0 || node == OP.root) {
//            match_length = 0;
//            current_node = OP.root;
//            i++;
//        } else {
//            // 找到当前 node 的 suffix link，然后跳过 β
//            auto suffix_node = node->slink;
//            current_node = suffix_node;
//            match_length = max(0, l - 1); // 这里简化处理
//            i++;
//        }
//    }

//    return ms_table;





int main(int argc, char *argv[])
{


    cmdline::parser parser;
    parser.add<string>("filepathA", 'a', "the path to input file A", false, "inputA.txt");
    parser.add<string>("filepathB", 'b', "the path to input file B", false, "inputB.txt");


    parser.add<int>("rangeThreshold", 'r', "the range of (a,b) determines call the naive or wavelet tree method", false, 512);


    parser.parse_check(argc, argv);

    string filenameA = parser.get<string>("filepathA");
    string filenameB = parser.get<string>("filepathB");
    int rangeThreshold = parser.get<int>("rangeThreshold");

//    string path = get_grandparent_directory(filenameA);
//    string name = get_filename(filenameA);

//    int tau = parser.get<int>("tau");

    cout<<"--------------------------------------------Information Board--------------------------------------------------------"<<endl;

    cout<<"Enabling OPST maximal method"<<endl;

    cout<< "Comparing "<<filenameA<<" and "<<filenameB<<endl;


    // read file A to input_vecA
    vector<int> input_vecA;
    readfile(filenameA, input_vecA);

    // read file A to input_vecB
    vector<int> input_vecB;
    readfile(filenameB, input_vecB);


    // OPST construction of input A
    auto Construction_start = std::chrono::high_resolution_clock::now();

    OPST_MS OP(input_vecA, rangeThreshold);

    auto Construction_end = std::chrono::high_resolution_clock::now();
    double time_Construction = std::chrono::duration_cast < std::chrono::microseconds > (Construction_end - Construction_start).count()*0.000001;

    OP.exportSuffixTreeToDot("max_no", false);
    OP.exportSuffixTreeToDot("max", true);

    oracle B_oracle(input_vecB, rangeThreshold);

    vector<int> MS_table(input_vecB.size(), 0);

    compute_matching_statistics(OP, B_oracle, MS_table);

    for (int i = 0; i < MS_table.size(); ++i) {
        cout << "MS[" << i << "] = " << MS_table[i] << endl;
    }





    cout<<"The input size: "<<OP.n<<endl;

    cout<<"Terminate_label (2 * n + 1) $ = " << OP.terminate_label<<endl;

    cout<<"Runtime only for OPST construction  = "<<time_Construction<<" s."<<endl;

//    cout<<"Runtime only used for mining "<<tau<<"-maximal order-preserving "<<tau<<"-frequent patterns: "<<time_Maximal<<" s."<<endl;
//    cout<<"Total runtime for OPST construction: "<<time_Construction<<" s."<<endl;



    cout<<"---------------------------------------------------------------------------------------------------------------------"<<endl;






    return 0;
}








