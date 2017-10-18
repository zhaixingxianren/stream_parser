#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

extern int g_level;
#define Log(level,...)   \
    do {                 \
        if(level > g_level){         \
            fprintf(stderr," [%s] ",__FUNCTION__); \
            fprintf(stderr,__VA_ARGS__); \
            fprintf(stderr,"\n");   \
            fflush(stderr)  ;        \
        }                           \
    }while(0)


/* 
 * xml parse wrapper
 */
class XMLWrapper {
public:
  XMLWrapper(const char* file){
    if(!file){
      return;    
    }
    Log(1,"%s",file);
    if( (doc=xmlParseFile(file)) == NULL ){
      xmlFreeDoc(doc);
      return;
    }
    if( (rootNode = xmlDocGetRootElement(doc)) == NULL){
      xmlFreeDoc(doc);
      return;
    }
  }

  ~XMLWrapper(){
    if(doc) xmlFreeDoc(doc);
  }

  xmlNodePtr getRootElement(){
    return rootNode; 
  }

  xmlNodePtr findNodeByName(xmlNodePtr rootnode, const xmlChar * nodename){
    xmlNodePtr node = rootnode;
    if(node == NULL || nodename == NULL){
        return NULL;
    }

    while(node != NULL){
        if(!xmlStrcmp(node->name,nodename)){
            Log(1,"found %s",nodename);
            return node;
        }else node = node->children;
    }
 
    return NULL;
  }

  xmlChar * getAttrValFromNode(xmlNodePtr node,const xmlChar * attrname){
      return xmlGetProp(node,attrname);
  }

private:
  xmlDocPtr doc;
  xmlNodePtr rootNode;
};
