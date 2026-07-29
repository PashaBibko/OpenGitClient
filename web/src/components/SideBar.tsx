import {BuildFileTree, SortTree, Tree} from "./FileTree.tsx";
import {FileStatusBreakdown} from "./MainContent.tsx";

export function SideBar({ fileChanges }: { fileChanges: FileStatusBreakdown[] }) {
    console.log(fileChanges.length);

    let tree = BuildFileTree(fileChanges);
    SortTree(tree);

    return (
        <Tree root={tree}/>
    )
}
