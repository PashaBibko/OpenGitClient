import {BuildFileTree, SortTree, Tree} from "./FileTree.tsx";
import {FileStatusBreakdown} from "./MainContent.tsx";
import {CommitBox} from "./CommitBox.tsx";

export function SideBar({ fileChanges }: { fileChanges: FileStatusBreakdown[] }) {
    let tree = BuildFileTree(fileChanges);
    SortTree(tree);

    return (
        <div className="flex flex-col h-full justify-between w-full">
            <Tree root={tree}/>
            <CommitBox/>
        </div>
    )
}
