/* 
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *  
 *  Copyright (C) 2007-2012 by TAKAGI Nobuhisa
 *  Copyright (C) 2010 by Meika Sugimoto
 *  Copyright (C) 2011-2014 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2011-2014 by FUJISOFT INCORPORATED, JAPAN
 *  Copyright (C) 2011-2013 by Spansion LLC, USA
 *  Copyright (C) 2011-2013 by NEC Communication Systems, Ltd., JAPAN
 *  Copyright (C) 2011-2014 by Panasonic Advanced Technology Development Co., Ltd., JAPAN
 *  Copyright (C) 2011-2014 by Renesas Electronics Corporation, JAPAN
 *  Copyright (C) 2011-2014 by Sunny Giken Inc., JAPAN
 *  Copyright (C) 2011-2014 by TOSHIBA CORPOTATION, JAPAN
 *  Copyright (C) 2011-2014 by Witz Corporation, JAPAN
 *  
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，AUTOSAR（AUTomotive Open System ARchitecture）仕
 *  様に基づいている．上記の許諾は，AUTOSARの知的財産権を許諾するもので
 *  はない．AUTOSARは，AUTOSAR仕様に基づいたソフトウェアを商用目的で利
 *  用する者に対して，AUTOSARパートナーになることを求めている．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: factory.cpp 179 2014-03-17 05:01:16Z nces-mtakada $
 */
 
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include "toppers/misc.hpp"
#include "toppers/global.hpp"
#include "toppers/csv.hpp"
#include "toppers/nm_symbol.hpp"
#include "toppers/s_record.hpp"
#include "toppers/diagnostics.hpp"
#include "toppers/macro_processor.hpp"
#include "toppers/io.hpp"
#include "toppers/cpp.hpp"
#include "toppers/xml/factory.hpp"
#include "toppers/xml/cfg1_out.hpp"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

namespace toppers
{
  namespace xml
  {
    namespace
    {
       container::object* find_object( std::string const& name , cfg1_out::xml_obj_map const& xml_map , cfg1_out const& cfg1out )
      {
        std::map< std::string, std::vector< container::object* > >::const_iterator p;
        std::vector< container::object* >::const_iterator q, q2;
        int fObject = 0;

        // 名前が一致するオブジェクトを検索
        string::size_type index = name.find_last_of("/");
        string container_name = name.substr(0,index);
        string parameter_name = name.substr(index+1);

        for(p = xml_map.begin() ; p != xml_map.end() ; p++)
        {
          for( q = (*p).second.begin() ; q != (*p).second.end() ; q++ )
          {
            if( parameter_name == (*q)->getObjName() )
            {
              string defname = cfg1out.search_info_pathname( (*q)->getDefName() );
              // フルパスのコンテナ名をチェック
              if ( defname.find( name ) != string::npos )
              {
                return (*q);
              }
              // フルパスと一致しなくてもオブジェクト名が一致しているので保存
              q2 = q;
              fObject++;
            }
          }
        }
        if ( fObject > 0 )
          return (*q2);
        else
          return NULL;
      }

      void add_tfvalue(factory::tf_e &tfvalue, string name, string value, int index)
      {
        factory::tf_element e;

        //std::cout << "add_tfvalue element[" << index <<"] name=[" << name << "] value=[" << value <<  "] " << std::endl;
        e.name = name;
        e.value = value;
        e.index = index;
        tfvalue.push_back(e);
      }

      std::string get_csvtype(csv::const_iterator& d_iter, csv::const_iterator& d_last)
      {
        std::string csvtype;

        if( d_iter < d_last)
        {
          if( ( *d_iter ).size() == 2  )
          {
            csvtype = "NORMAL";
          }
          else if( ( *d_iter ).size() < 2 || 4 < ( *d_iter ).size() )
          {
            warning( _("%1% is invalid format (%2%)\n"), ( *d_iter )[ 0 ], get_global_string( "XML_XMLEvaluateFile" ) );
          }
          else
          {
            csvtype = ( *d_iter )[ 2 ];
          }
        }
        return csvtype;
      }

      std::string dom_search_node_fullpath(DOMNode *node)
      {
        std::string path, nodeName;
        DOMNodeList *nodeList = node->getParentNode()->getChildNodes();

        int i = nodeList->getLength();
        for(int j=0 ; j<i ; j++)
        {
          DOMNode *child = nodeList->item(j);

          nodeName = XMLString::transcode(child->getNodeName());
          //std::cout << "dom_search_node_fullpath node name[" << j << "] = [" << nodeName << "]" << std::endl;
          if( nodeName == "SHORT-NAME")
          {
            path = XMLString::transcode(child->getTextContent());
            //std::cout << "dom_search_node_fullpath node vale =[" << path << "]" << std::endl;
          }
        }

        if( nodeName != "AUTOSAR")
        {
          std::string nodeValue;
          if(!path.empty())
          {
            nodeValue = "/" + path;
          }
          path = dom_search_node_fullpath(node->getParentNode()) + nodeValue;
        }
        return path;
      }

      void dom_xml_parse(csv::const_iterator& d_iter, csv::const_iterator& d_last, xercesc::DOMDocument *doc, xercesc::DOMNode *cnode,
       factory::tf_e &tfvalue, std::string &befortype, int &childnum, std::string groupname = "", int index = 0)
      {
        //std::cout << "dom_xml_parse Node name is " << XMLString::transcode( cnode->getNodeName() ) << "." << std::endl;
        // ロケーションパス，tf変数名，タイプ(NORMAL/PARENT/CHILD)以外の記述がされている場合はワーニングを出して探索を行わない
        std::string evaltype = get_csvtype(d_iter, d_last);

        if( (befortype == "NORMAL" && evaltype == "CHILD") || (befortype == "PARENT" && evaltype == "NORMAL") )
        {
          //warning( _("'%1%' is invalid state type from '%2%' to '%3%'. \n"), ( *d_iter )[ 0 ], befortype, evaltype );
          return;
        }

        // 子要素からのもどり
        if( (befortype == "CHILD" && evaltype == "NORMAL") || (befortype == "CHILD" && evaltype == "PARENT") || (befortype == "PARENT" && evaltype == "PARENT"))
        {
          childnum = 0;
        }
        if( evaltype == "CHILD" )
        {
          childnum++;
        }

        //std::cout << "dom_xml_parse start beforetype=[" << befortype << "] evaltype=[" << evaltype <<  "] name=[" << groupname << "] Str=[" << ( *d_iter )[ 0 ] << "]" << std::endl;
        // XMLの評価
        XMLCh* xpathStr = XMLString::transcode( ( *d_iter )[ 0 ].c_str() );
        try
        {
          DOMXPathNSResolver* resolver=doc->createNSResolver(cnode);
          DOMXPathResult* result=doc->evaluate(
           xpathStr,
           cnode,
           resolver,
           DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE ,
           NULL);

          XMLSize_t nLength = result->getSnapshotLength();
          if( nLength == 0 ) 
          {
            if( get_global_string( "XML_VerboseMessage" ) == "TRUE" )
            {
              const XMLCh *moji = cnode->getNodeName();
              warning( _("element '%1%' is no data corresponding to XPath. Parent element is '%2%'.\n"), ( *d_iter )[ 0 ], XMLString::transcode(moji) );              
            }
          }
          else
          {
            for(int i = 0; i < nLength ; i++)
            {
              evaltype = get_csvtype(d_iter, d_last);
              result->snapshotItem(i);

              if(evaltype == "PARENT")
              {
                groupname = ( *d_iter )[ 1 ];
                befortype = evaltype;
                while( d_iter < d_last  )
                {
                  ++d_iter;
                  evaltype = get_csvtype(d_iter, d_last);

                  if(evaltype != "CHILD")
                  {
                    break;
                  }
                  dom_xml_parse(d_iter, d_last, doc, result->getNodeValue(), tfvalue, befortype, childnum, groupname, i);
                }
                if( i+1 < nLength)
                {
                  d_iter -= (childnum + 1);
                }
                else
                {
                  d_iter--;
                  befortype = evaltype;
                }
                childnum = 0;
                groupname = "";
              }
              else
              {
                const XMLCh *moji = result->getNodeValue()->getTextContent();
                string name;
                if( !groupname.empty() )
                {
                  name = groupname + "." + (*d_iter)[1];
                }
                else
                {
                  name = (*d_iter)[1];
                }
                if(evaltype == "CHILD")
                {
                  if(( *d_iter ).size() == 4)
                  {
                    if((std::string)(*d_iter)[3] == "PATH")
                    {
                      add_tfvalue(tfvalue, name, dom_search_node_fullpath(result->getNodeValue()), index);
                    }
                    else
                    {
                      warning( _("%1% is invalid format (%2%)\n"), ( *d_iter )[ 0 ], get_global_string( "XML_XMLEvaluateFile" ) );
                    }
                  }
                  else
                  {
                    add_tfvalue(tfvalue, name, XMLString::transcode(moji), index);
                  }
                }
                else
                {
                  add_tfvalue(tfvalue, name, XMLString::transcode(moji), i);                  
                }
              }
            }
            result->release();
            resolver->release();
          }
        }
        catch (const DOMXPathException& e)
        {
          warning( _("An error occurred during processing of the XPath expression. Msg is: %\n"), StrX(e.getMessage()) );
        }
        catch (const DOMException& e)
        {
          warning( _("An error occurred during processing of the XPath expression. Msg is: %\n"), StrX(e.getMessage()) );
        }
        XMLString::release(&xpathStr);
      }

      // カーネルオブジェクト生成・定義用静的APIの各パラメータをマクロプロセッサの変数として設定する。
      void set_object_vars( cfg1_out const& cfg1out, cfg1_out::xml_obj_map const& xml_map, macro_processor& mproc )
      {
        typedef macro_processor::element element;
        typedef macro_processor::var_t var_t;
        std::map< std::string, var_t > order_list_map;
        std::map< std::string, long > id_map;

        using namespace toppers::xml::container;
        std::map< std::string, std::vector<object*> >::const_iterator p;
        std::vector<object*>::const_iterator q;
        std::vector<parameter*>::const_iterator r;

        std::size_t sizeof_signed_t;
        std::size_t sizeof_pointer;
        nm_symbol::entry nm_entry;
        if ( !get_global_bool( "omit-symbol" ) )
        {
          nm_entry = cfg1out.get_syms()->find( "TOPPERS_cfg_sizeof_signed_t" );
          sizeof_signed_t = static_cast< std::size_t >( cfg1out.get_srec()->get_value( nm_entry.address, 4, cfg1out.is_little_endian() ) );

          nm_entry = cfg1out.get_syms()->find( "TOPPERS_cfg_sizeof_pointer" );
          sizeof_pointer = static_cast< std::size_t >( cfg1out.get_srec()->get_value( nm_entry.address, 4, cfg1out.is_little_endian() ) );
        }

        for ( p = xml_map.begin() ; p != xml_map.end() ; p++)
        {
          element e;
          var_t t;
          string name;
          long obj_id;
          long param_id;

          // パラメータの値代入
          for(q = (*p).second.begin() ; q != (*p).second.end() ; q++)
          {
            std::map< std::string, var_t > obj_parameter;
            std::map< std::string, var_t >::iterator s;
            obj_id = (*q)->getId();
            name = (*p).first;
            std::map<std::string , long> id_count;
            std::map<std::string , long>::iterator id_iter;

            // 値の取り出し
            e.i = obj_id;
            e.s = (*q)->getObjName();
            // 出現順リスト用の情報作成
            order_list_map[ name ].push_back(e);
            // オブジェクト自身の値代入
            mproc.set_var( name, obj_id, var_t( 1, e ) );
            // 親コンテナがある場合はその情報を登録
            if( (*q)->getParent() != NULL )
            {
              e.s = (*q)->getParent()->getObjName();
              e.i = (*q)->getParent()->getId();
              mproc.set_var( name +".PARENT", obj_id, var_t( 1, e ) );
            }

            // オブジェクトメンバの値代入
            for(r = (*q)->getParams()->begin() ; r != (*q)->getParams()->end() ; r++)
            {
              name = (*p).first+ string(".") + (*r)->getDefName();
              e.s = (*r)->getValue();

              if(e.s == string("") || (*r)->getDefName().find("/") != string::npos )
              {
                //std::cout << "set_object_vars name:[" << name << "] e.s:[" << e.s << "]" << std::endl;
                continue;
              }

              // メンバのIDを検索
              id_iter = id_count.find(name);
              if(id_iter == id_count.end())
              {
                id_count[name] = 0;
                param_id = 0;
              }
              else
              {
                param_id = (*id_iter).second + 1;
                id_count[name] = (*id_iter).second + 1;
              }

              if((*r)->getType() == TYPE_INT || (*r)->getType() == TYPE_FLOAT)
              {
                string value_str( (*r)->getValue() );
                try
                {
                  e.i = boost::lexical_cast< std::tr1::int64_t >( value_str );
                }
                catch( std::exception& exception )
                {
                  string contanerDefName( (*r)->getParent()->getDefName() );
                  string paramDefName( (*r)->getDefName() );
                  string grandObjName;

                  boost::replace_all(contanerDefName, ".", "_");
                  boost::replace_all(paramDefName, ".", "_");
                  if( (*r)->getParent()->getParent() != NULL )
                  {
                    grandObjName = (*r)->getParent()->getParent()->getObjName();
                  }

                  if ( !get_global_bool( "omit-symbol" ) )
                  {
                    nm_entry = cfg1out.get_syms()->find( "TOPPERS_cfg_valueof_" + contanerDefName + "_" + paramDefName + "_" + (*r)->getParent()->getObjName() + "_" + grandObjName );
                    if ( nm_entry.type >= 0 )
                    {
                      e.i = cfg1out.get_srec()->get_value( nm_entry.address, sizeof_signed_t, cfg1out.is_little_endian() );
                    }
                    else
                    {
                      e.i = 0;
                    }
                  }
                  else
                  {
                    e.i = 0;
                  }
                }
              }
              else if( (*r)->getType() == TYPE_REF)
              {
                object *obj;
                string refefence_obj_type;

                // オブジェクトIDの探索
                e.i = 0;
                obj = find_object( (*r)->getValue() , xml_map, cfg1out );
                if(obj != NULL)
                {
                  e.i = obj->getId();
                  e.s = e.s.substr(e.s.find_last_of("/")+1);
                }
                else
                {
                  toppers::fatal( _( "(%1%:%2%) : not match Refernce ID filds in `%3%\'" ), (*r)->getFileName(), (*r)->getLine(), (*r)->getValue() );
                }
              }
              else if( (*r)->getType() == TYPE_BOOLEAN )
              {
                string value_str( (*r)->getValue() );
                if(value_str == "1" || value_str == "TRUE" || value_str == "true")
                {
                  e.i = 1;
                }
                else
                {
                  e.i = 0;
                }
              }
              else
              {
                e.i = 0;
              }

              if((*r)->getType() != TYPE_UNKNOWN)
              {
                obj_parameter[name].push_back(e);

                // パラメータタグが出現した行番号
                {
                  element e;
                  e.s = (*q)->getFileName();
                  e.i = (*r)->getLine() + 1;

                  mproc.set_var( name + ".TEXT_LINE", obj_id, var_t( 1, e ) );
                }
              }

            }

            for(s = obj_parameter.begin() ; s != obj_parameter.end() ; s++)
            {
              mproc.set_var((*s).first , obj_id , (*s).second );
            }

            // コンテナタグが出現した行番号
            {
              element e;
              e.s = (*q)->getFileName();
              e.i = (*q)->getLine() + 1;

              mproc.set_var( (*p).first + ".TEXT_LINE", obj_id, var_t( 1, e ) );

              // コンテナの兄弟コンテナ数
              //e.s = (*q)->getSiblings();
              //e.i = (*q)->getSiblings();
              //mproc.set_var( (*p).first + ".SIBLINGS", obj_id, var_t( 1, e ) );
            }

          }

        }
        // 汎用XMLの要素をXPathによるevalateでマクロプロセッサの変数として設定する
        if( !get_global_string( "XML_XMLEvaluateFile" ).empty() )
        {
          try
          {
            XMLPlatformUtils::Initialize();
          }
          catch (const XMLException& e)
          {
            fatal( _("Error during initialization! Message:\n%" ), toNative(e.getMessage()));
          }

          // Instantiate the DOM parser.
          static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
          DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(gLS);
          DOMLSParser       *parser = ((DOMImplementationLS*)impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
          DOMConfiguration  *config = parser->getDomConfig();

          config->setParameter(XMLUni::fgDOMNamespaces, true);
          config->setParameter(XMLUni::fgXercesSchema, false);
          config->setParameter(XMLUni::fgXercesHandleMultipleImports, true);
          config->setParameter(XMLUni::fgXercesSchemaFullChecking, true);
          config->setParameter(XMLUni::fgDOMValidateIfSchema, true);
          config->setParameter(XMLUni::fgDOMDatatypeNormalization, true);

          DOMCfgErrorHandler errorHandler;
          config->setParameter(XMLUni::fgDOMErrorHandler, &errorHandler);

          std::string exschema( get_global_string( "cfg-directory" ) + "/" + get_global_string( "XML_Schema" ) );
          XMLCh* xsdFile (XMLString::transcode( exschema.c_str() ));
          parser->loadGrammar(xsdFile, Grammar::SchemaGrammarType, true);

          errorHandler.resetErrors();

          XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = 0;
          
          std::list<string> urifiles;
          boost::split( urifiles, get_global_string( "input-file" ), boost::is_space() );

          factory::tf_e tfvalue;
          // 指定したXMLファイルが複数ある場合には複数回処理を繰り返す
          BOOST_FOREACH( string urifile, urifiles )
          {
            std::string filebuf;

            // ファイルの存在チェック
            read( urifile, filebuf );

            // XMLファイルのパース
            try
            {
              parser->resetDocumentPool();
              doc = parser->parseURI( urifile.c_str() );
            }
            catch (const XMLException& e)
            {
              warning( _("\nError during parsing: '%'\nException message is:  \n%\n"), urifile, StrX(e.getMessage()) );
            }
            catch (const DOMException& e)
            {
              const unsigned int maxChars = 2047;
              XMLCh errText[maxChars + 1];

              warning( _("\nDOM Error during parsing: '%'\nDOMException message is:  \n%\n"), urifile, StrX(e.getMessage()) );

              if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
                warning( _("Message is: '%'\n"), StrX(errText) );
            }
            
            // CSVファイルのパース
            read( boost::filesystem::absolute( get_global_string( "cfg-directory" ).c_str() ).string() + "/" + get_global_string( "XML_XMLEvaluateFile" ) , filebuf);
            csv data( filebuf.begin(), filebuf.end() );

            // CSVファイルからXPathのリスト記述分だけXMLのDOM探索を行う
            string type = "NORMAL";
            int childnum = 0;
            for ( csv::const_iterator d_iter( data.begin() ), d_last( data.end() );
              d_iter != d_last;
              ++d_iter )
            {
              dom_xml_parse( d_iter, d_last, doc, (DOMNode*)doc->getDocumentElement(), tfvalue, type, childnum );
            }
          }
          delete parser;

          map<string, int> tf_map;
          // 格納された要素をtf変数名として設定する
          for( std::tr1::int64_t i = 0 ; i < tfvalue.size() ; i++ )
          {
            macro_processor::element e;
            const std::string pathStr= "_PATH";
            int last_index;   // 複数ファイルで同一コンテナの場合，前のインデックス値を付加する必要がある

            if(tfvalue[i].index == 0)
            {
              map<string, int>::iterator tf_itr = tf_map.find( tfvalue[i].name );
              if(tf_itr != tf_map.end())
              {
                last_index = tf_itr->second;
              }
              else
              {
                last_index = 0;
              }
            }
            //std::cout << "mproc.set_var[" << i << "] index:[" << tfvalue[i].index << "] last_index:[" << last_index << "] value:[" << tfvalue[i].value << "] name:[" << tfvalue[i].name << "] " << std::endl;
            tf_map[tfvalue[i].name] = tfvalue[i].index + last_index + 1;

            e.s = tfvalue[i].value;
            mproc.set_var( tfvalue[i].name, tfvalue[i].index + last_index, var_t( 1, e ) );

            // PATH種別オプションにはID_LISTを付与する
            string::size_type pathindex = tfvalue[i].name.rfind( pathStr );
            if( pathindex == tfvalue[i].name.length() - pathStr.length() )
            {
              //ID番号リストへ登録
              e.i = tfvalue[i].index + last_index + 1;
              order_list_map[ tfvalue[i].name ].push_back(e);
            }
          }
        }

        // 順序リストの作成
        for ( std::map< std::string, var_t >::const_iterator iter( order_list_map.begin() ), last( order_list_map.end() );
          iter != last;
          ++iter )
        {
          // 出現順リスト $OBJ.ORDER_LIST$ -- ID番号の並び
          mproc.set_var( iter->first + ".ORDER_LIST" , iter->second );
          var_t rorder_list( iter->second );

          // 逆順リスト $OBJ.RORDER_LIST$ -- ID番号の並び
          std::reverse( rorder_list.begin(), rorder_list.end() );
          mproc.set_var( iter->first + ".RORDER_LIST" , rorder_list );

          // ID番号リスト $OBJ.ID_LIST$ -- ID番号の並び
          var_t id_list( iter->second );
          std::sort( id_list.begin(), id_list.end() );
          mproc.set_var( iter->first + ".ID_LIST" , id_list );
        }

        element external_id;
        external_id.i = get_global_bool( "external-id" );
        mproc.set_var( "USE_EXTERNAL_ID", var_t( 1, external_id ) );
      }


      // プラットフォーム・コンパイラ依存の値をマクロプロセッサの変数として設定する。
      void set_platform_vars( cfg1_out const& cfg1out, macro_processor& mproc )
      {
        typedef macro_processor::element element;
        typedef macro_processor::var_t var_t;

        cfg1_out::cfg1_def_table const* def_table = cfg1out.get_def_table();
        std::size_t sizeof_signed_t;
        std::size_t sizeof_pointer;

        static cfg1_out::cfg1_def_t const limit_defs[] = {};

        nm_symbol::entry nm_entry = cfg1out.get_syms()->find( "TOPPERS_cfg_sizeof_signed_t" );
        sizeof_signed_t = static_cast< std::size_t >( cfg1out.get_srec()->get_value( nm_entry.address, 4, cfg1out.is_little_endian() ) );

        nm_entry = cfg1out.get_syms()->find( "TOPPERS_cfg_sizeof_pointer" );
        sizeof_pointer = static_cast< std::size_t >( cfg1out.get_srec()->get_value( nm_entry.address, 4, cfg1out.is_little_endian() ) );

        for ( std::size_t i = 0; i < sizeof limit_defs / sizeof limit_defs[ 0 ]; ++i )
        {
          element e;
          e.s = limit_defs[ i ].expression;
          nm_entry = cfg1out.get_syms()->find( limit_defs[ i ].name );
          std::tr1::int64_t value = cfg1out.get_srec()->get_value( nm_entry.address, sizeof_signed_t, cfg1out.is_little_endian() );
          if ( sizeof_signed_t < 8 && limit_defs[ i ].is_signed )
          {
            value = cfg1_out::make_signed( static_cast< std::tr1::uint32_t >( value ) );
          }
          mproc.set_var( e.s, var_t( 1, e ) );
        }

        for ( cfg1_out::cfg1_def_table::const_iterator iter( def_table->begin() ), last( def_table->end() );
          iter != last;
          ++iter )
        {
          element e;
          std::tr1::int64_t value;

          nm_entry = cfg1out.get_syms()->find( "TOPPERS_cfg_" + iter->name );
          if ( nm_entry.type >= 0 )
          {
            if ( !iter->expression.empty() && iter->expression[ 0 ] == '@' )  // 式が'@'で始まる場合はアドレス定数式
            {
              value = cfg1out.get_srec()->get_value( nm_entry.address, sizeof_pointer, cfg1out.is_little_endian() );
              if ( sizeof_signed_t < 8 && iter->is_signed )
              {
                value = cfg1_out::make_signed( static_cast< std::tr1::uint32_t >( value ) );
              }

              // 先ほど取り出したアドレスを使って間接参照
              value = cfg1out.get_srec()->get_value( value, 8, cfg1out.is_little_endian() );  // 取り出す値は型に関係なく常に8バイト
              if ( sizeof_signed_t < 8 && iter->is_signed )
              {
                value = cfg1_out::make_signed( static_cast< std::tr1::uint32_t >( value ) );
              }
              e.s = iter->expression.c_str() + 1; // 先頭の'@'を除去
            }
            else  // アドレスではない通常の整数定数式
            {
              value = cfg1out.get_srec()->get_value( nm_entry.address, sizeof_signed_t, cfg1out.is_little_endian() );
              if ( sizeof_signed_t < 8 && iter->is_signed )
              {
                value = cfg1_out::make_signed( static_cast< std::tr1::uint32_t >( value ) );
              }
              e.s = iter->expression;
            }
            e.i = value;
            mproc.set_var( iter->name, var_t( 1, e ) );
          }
        }

        // バイトオーダー
        {
          bool little_endian = cfg1out.is_little_endian();
          element e;
          e.i = little_endian;
          mproc.set_var( "LITTLE_ENDIAN", var_t( 1, e ) );

          e.i = !little_endian;
          mproc.set_var( "BIG_ENDIAN", var_t( 1, e ) );
        }
      }

    }

    //! コンストラクタ
    factory::factory( std::string const& kernel )
      : kernel_( tolower( kernel ) )
    {
    }

    //! デストラクタ
    factory::~factory()
    {
    }

    //! サポートしているオブジェクト情報の取得
    std::map<std::string, toppers::xml::info> const* factory::get_container_info_map() const
    {
      // CSVからContainer情報を読み取り、登録するためのローカルクラス
      struct init_t
      {
        init_t()
        {
          boost::any t = global( "api-table" );
          if ( !t.empty() )
          {
            std::list<std::string> moduleNames;
            boost::split(moduleNames, get_global_string( "XML_ModuleName" ), boost::is_any_of(",") );
            std::string containerLocation( get_global_string( "XML_ContainerPath" ) );
            std::vector< std::string > api_tables( boost::any_cast< std::vector< std::string >& >( t ) );
            for ( std::vector< std::string >::const_iterator iter( api_tables.begin() ), last( api_tables.end() );
                  iter != last;
                  ++iter )
            {
              std::string buf;
              std::string api_table_filename = *iter;
              read( api_table_filename.c_str(), buf );
              csv data( buf.begin(), buf.end() );
              for ( csv::const_iterator d_iter( data.begin() ), d_last( data.end() );
                    d_iter != d_last;
                    ++d_iter )
              {
                int searchFlag = 0;
                BOOST_FOREACH(std::string module, moduleNames)
                {
                  //toppers::warning( _( "search container = `%1%\'" ), containerLocation + "/" + module );
                  if ( (*d_iter)[0].find( containerLocation + "/" + module ) != string::npos )
                  {
                    searchFlag++;
                  }
                }
                if ( searchFlag > 0 )
                {
                  csv::size_type len = d_iter->size();
                  if ( len < 2 )  // container full-path, container rename,は必須要素
                  {
                    toppers::fatal( _( "too little fields in `%1%\'" ), *iter );
                  }

                  toppers::xml::info  xml_info= { 0 };
                  try
                  {
                    char* s;
                    s = new char[ ( *d_iter )[ 1 ].size() + 1 ];
                    std::strcpy( s, ( *d_iter )[ 1 ].c_str() );
                    xml_info.tfname = s;

                    s = new char[ ( *d_iter )[ 2 ].size() + 1 ];
                    std::strcpy( s, ( *d_iter )[ 2 ].c_str() );
                    xml_info.type = s;
                    if( strlen(s) )
                    {
                      if( !strcmp( s, "INT" ) )
                          xml_info.type_enum = toppers::xml::container::TYPE_INT;
                      else if( !strcmp( s, "FLOAT" ) )
                          xml_info.type_enum = toppers::xml::container::TYPE_FLOAT;
                      else if( !strcmp( s, "STRING" ) || !strcmp( s, "+STRING" ) )
                          xml_info.type_enum = toppers::xml::container::TYPE_STRING;
                      else if( !strcmp( s, "BOOLEAN" ) )
                          xml_info.type_enum = toppers::xml::container::TYPE_BOOLEAN;
                      else if( !strcmp( s, "ENUM" ) )
                          xml_info.type_enum = toppers::xml::container::TYPE_ENUM;
                      else if( !strcmp( s, "REF" ) )
                          xml_info.type_enum = toppers::xml::container::TYPE_REF;
                      else if( !strcmp( s, "FUNCTION" ) )
                          xml_info.type_enum = toppers::xml::container::TYPE_FUNCTION;
                      else if( !strcmp( s, "INCLUDE" ) || !strcmp( s, "+INCLUDE" ))
                          xml_info.type_enum = toppers::xml::container::TYPE_INCLUDE;
                    }

                    if ( len >= 4 && !( *d_iter )[ 3 ].empty() )
                    {
                      try
                      {
                        string multi = ( *d_iter )[ 3 ].c_str();
                        xml_info.multimin = boost::lexical_cast<int>(multi);
                        xml_info.multimax = xml_info.multimin;
                      }
                      catch(...)
                      {
                        toppers::fatal( _( "Invalid parameter fields in `%1%\'" ), *iter );
                      }
                    }
                    if ( len >= 5 && !( *d_iter )[ 4 ].empty() )
                    {
                      if("*" == ( *d_iter )[ 4 ])
                      {
                        xml_info.multimax = -1;  // unsigned intの最大値
                      }
                      else
                      {
                        try
                        {
                          xml_info.multimax = boost::lexical_cast<int>(( *d_iter )[ 4 ].c_str());
                        }
                        catch(...)
                        {
                          toppers::fatal( _( "Invalid parameter fields in `%1%\'" ), *iter );
                        }
                      }
                    }
                    container_table_[ (*d_iter)[0].c_str() ] = xml_info;
                  }
                  catch( ... )
                  {
                    delete[] xml_info.type;
                    delete[] xml_info.tfname;
                    throw;
                  }
                }
                else 
                {
                  //toppers::warning( _( "pass container = `%1%\'" ), (*d_iter)[1].c_str() );
                }
              }
            }
          }
        }

        ~init_t()
        {
          for ( std::map< std::string, toppers::xml::info >::const_iterator iter( container_table_.begin() ), last( container_table_.end() );
                iter != last;
                ++iter )
          {
            delete[] iter->second.type;
            delete[] iter->second.tfname;
          }
        }

        std::map< std::string, toppers::xml::info > container_table_;
      };
      static init_t init;
      if ( init.container_table_.empty() )
      {
        toppers::fatal( _( "api-table file is empty or Container Location path is out of range." ) );
      }
      std::map< std::string, toppers::xml::info > const* result = &init.container_table_;
      return result;
    }

    /*!
     * \brief   cfg1_out.c への出力情報テーブルの生成
     * \return  生成した cfg1_out::cfg1_def_table オブジェクトへのポインタ
     * \note    この関数が返すポインタは delete してはならない
     *
     * --cfg1-def-table オプションで指定したファイルから、cfg1_out.c へ出力する情報を読み取り、
     * cfg1_out::cfg1_def_table オブジェクトを生成する。
     *
     * CSV の形式は以下の通り
     *
     *    シンボル名,式[,s|signed]
     *
     * 末尾の s または signed は省略可能。省略時は符号無し整数とみなす。s または signed 指定時は
     * 符号付き整数とみなす。\n
     * 「式」の最初に # があれば前処理式とみなす。
     */
    cfg1_out::cfg1_def_table const* factory::get_cfg1_def_table() const
    {
      struct init_t
      {
        init_t()
        {
          boost::any t = global( "cfg1-def-table" );
          if ( !t.empty() )
          {
            std::vector< std::string > cfg1_def_table = boost::any_cast< std::vector< std::string >& >( t );
            for ( std::vector< std::string >::const_iterator iter( cfg1_def_table.begin() ), last( cfg1_def_table.end() );
                  iter != last;
                  ++iter )
            {
              std::string buf;
              read( iter->c_str(), buf );
              csv data( buf.begin(), buf.end() );
              for ( csv::const_iterator d_iter( data.begin() ), d_last( data.end() );
                    d_iter != d_last;
                    ++d_iter )
              {
                csv::size_type len = d_iter->size();
                if ( len < 2 )
                {
                  toppers::fatal( _( "too little fields in `%1%\'" ), *iter );
                }
                cfg1_out::cfg1_def_t def = { 0 };
                def.name = ( *d_iter )[ 0 ];
                def.expression = ( *d_iter )[ 1 ];
                if ( len >= 3 )
                {
                  std::string is_signed( ( *d_iter )[ 2 ] );
                  def.is_signed = ( is_signed == "s" || is_signed == "signed" );
                }
                if ( len >= 4)
                {
                  def.value1 = ( *d_iter )[ 3 ];
                }
                if ( len >= 5)
                {
                  def.value2 = ( *d_iter )[ 4 ];
                }
                cfg1_def_table_.push_back( def );
              }
            }
          }
        }
        cfg1_out::cfg1_def_table cfg1_def_table_;
      };
      static init_t init;
      cfg1_out::cfg1_def_table const* result = &init.cfg1_def_table_;
      return result;
    }

    //! オブジェクトの交換
    void factory::do_swap( factory& other )
    {
      kernel_.swap( other.kernel_ );
    }

    /*!
     *  \brief  マクロプロセッサの生成
     *  \param[in]  cfg1out cfg1_out オブジェクト
     *  \param[in]  api_map .cfg ファイルに記述された静的API情報
     *  \return     マクロプロセッサへのポインタ
     *  \note   このメンバ関数は従来仕様（ソフトウェア部品非対応版）の温存のためにそのまま残す。
     */
    std::auto_ptr< macro_processor > factory::do_create_macro_processor( cfg1_out const& cfg1out, cfg1_out::xml_obj_map const& xml_map ) const
    {
      typedef macro_processor::element element;
      typedef macro_processor::var_t var_t;
      std::auto_ptr< macro_processor > mproc( new macro_processor );
      element e;

      e.s = " ";    mproc->set_var( "SPC", var_t( 1, e ) );  // $SPC$
      e.s = "\t";   mproc->set_var( "TAB", var_t( 1, e ) );  // $TAB$
      e.s = "\n";   mproc->set_var( "NL",  var_t( 1, e ) );  // $NL$

      // バージョン情報
      e.s = toppers::get_global_string( "version" );
      e.i = toppers::get_global< std::tr1::int64_t >( "timestamp" );
      mproc->set_var( "CFG_VERSION", var_t( 1, e ) );   // $CFG_VERSION$

      // パス情報
      e.s = boost::lexical_cast< string >( toppers::get_global< int >( "pass" ) );
      e.i = toppers::get_global< int >( "pass" );
      mproc->set_var( "CFG_PASS", var_t( 1, e ) );

      // CFG_XML変数
      e.s = "1";
      e.i = 1;
      mproc->set_var( "CFG_XML",  var_t( 1, e ) );

      // その他の組み込み変数の設定
      set_object_vars( cfg1out, xml_map, *mproc );
      if ( !get_global_bool( "omit-symbol" ) )
      {
        set_platform_vars( cfg1out, *mproc );
      }
      e.s = cfg1out.get_includes();
      e.i = boost::none;
      mproc->set_var( "INCLUDES", var_t( 1, e ) );
      return mproc;
    }

    std::auto_ptr< cfg1_out > factory::do_create_cfg1_out( std::string const& filename ) const
    {
      return std::auto_ptr< xml::cfg1_out >( new cfg1_out( filename, get_cfg1_def_table() ) );
    }
    std::auto_ptr< checker > factory::do_create_checker() const
    {
      return std::auto_ptr< xml::checker >( new checker );
    }

  }
}

DOMCfgErrorHandler::DOMCfgErrorHandler() :

    fSawErrors(false)
{
}

DOMCfgErrorHandler::~DOMCfgErrorHandler()
{
}


// ---------------------------------------------------------------------------
//  DOMCfgHandlers: Overrides of the DOM ErrorHandler interface
// ---------------------------------------------------------------------------
bool DOMCfgErrorHandler::handleError(const DOMError& domError)
{
    fSawErrors = true;
    if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
        XERCES_STD_QUALIFIER cerr << "\nWarning at file ";
    else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
        XERCES_STD_QUALIFIER cerr << "\nError at file ";
    else
        XERCES_STD_QUALIFIER cerr << "\nFatal Error at file ";

    XERCES_STD_QUALIFIER cerr << StrX(domError.getLocation()->getURI())
         << ", line " << domError.getLocation()->getLineNumber()
         << ", char " << domError.getLocation()->getColumnNumber()
         << "\n  Message: " << StrX(domError.getMessage()) << XERCES_STD_QUALIFIER endl;

    return true;
}

void DOMCfgErrorHandler::resetErrors()
{
    fSawErrors = false;
}

