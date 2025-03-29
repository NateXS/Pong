#include "../../include/renderable/Scene.h"
#include "../../include/renderable/Sprite.h"
#include "../../include/Console.h"

Scene::Scene(gfxScreen_t screen, u32 backgroundColor_){
    name = "Scene";
    renderTarget = C2D_CreateScreenTarget(screen, GFX_LEFT);
    backgroundColor = backgroundColor_;
}


string* Scene::getExtended(){
    string* ext = new string[4];
    ext[0] = "Classified";
    ext[1] = "TreeElement";
    ext[2] = "Renderable";
    return ext;
}

void Scene::render(){
    if(!visible) return;
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(renderTarget, backgroundColor);
    C2D_SceneBegin(renderTarget);

    vector<TreeElement *> descendants = getDescendants();
    for(TreeElement *desc : descendants){
        if(instanceof(desc,"Renderable")){
            /*
            // render() is called on Renderable instead of the actual desc's class.
            Renderable *rend = (Renderable*)desc;
            rend->render();
            */
           // TODO: Find a way to make this universal instead of making every combination.
           if(instanceof(desc,"Sprite")){
                Sprite *rend = (Sprite*)desc;
                rend->render();
           }
           else if(instanceof(desc,"Console")){
                Console *rend = (Console*)desc;
                rend->render();
           }
        }
    }
    //descendants.clear();
    C3D_FrameEnd(0);
}
