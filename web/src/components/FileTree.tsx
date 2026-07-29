import {useLayoutEffect, useRef, useState} from "react";
import {FileStatusBreakdown} from "./MainContent.tsx";
import {useAppState} from "../AppState.ts";

const ROW_HEIGHT = 28; // px — just the visual label row, not the whole subtree
const INDENT = 20; // px width of the connector gutter

type FileNode = {
    type: "file";
    name: string;
    path: string;
    data: FileStatusBreakdown;
}

export type FolderNode = {
    type: "folder";
    name: string;
    path: string;
    children: TreeNode[];
};

type TreeNode = FileNode | FolderNode;

export function BuildFileTree(items: FileStatusBreakdown[]): FolderNode {
    const root: FolderNode = {
        type: "folder",
        name: "root",
        path: "",
        children: []
    };

    for (const item of items) {
        const parts = item.FileLocation.split("/").filter(Boolean);
        let current: FolderNode = root;

        parts.forEach((part, index) => {
            const isLastPart = index === parts.length - 1;
            const currentPath = parts.slice(0, index + 1).join("/");

            if (isLastPart) {
                current.children.push({
                    type: "file",
                    name: part,
                    path: currentPath,
                    data: item,
                });
            } else {
                let folder = current.children.find((child): child is FolderNode =>
                    child.type === "folder" && child.name === part);

                if (!folder) {
                    folder = {
                        type: "folder",
                        name: part,
                        path: currentPath,
                        children: []
                    }
                    current.children.push(folder);
                }

                current = folder;
            }
        });
    }

    return root;
}

export function SortTree(node: FolderNode) {
    node.children.sort((a, b) => {
        if (a.type !== b.type) return a.type === "folder" ? -1 : 1;
        return a.name.localeCompare(b.name);
    })

    node.children.forEach((child) => {
        if (child.type === "folder") SortTree(child);
    })
}

function TreeChildren({ nodes }: { nodes: TreeNode[] }) {
    const containerRef = useRef<HTMLDivElement>(null);
    const rowRefs = useRef<(HTMLDivElement | null)[]>([]);
    const [geometry, setGeometry] = useState<{ anchors: number[]; height: number }>({
        anchors: [],
        height: 0,
    });

    useLayoutEffect(() => {
        const container = containerRef.current;
        if (!container) return;

        const measure = () => {
            const containerTop = container.getBoundingClientRect().top;
            const anchors = rowRefs.current.map((row) => {
                if (!row) return 0;
                const rect = row.getBoundingClientRect();
                return rect.top - containerTop + rect.height / 2;
            });
            setGeometry({ anchors, height: container.scrollHeight });
        };

        measure();

        const ro = new ResizeObserver(measure);
        ro.observe(container);
        return () => ro.disconnect();
    }, [nodes]);

    return (
        <div ref={containerRef} className="relative">
            <svg className="pointer-events-none absolute left-0 top-0" width={INDENT} height={geometry.height}>
                {geometry.anchors.length > 0 && (
                    <line
                        x1={INDENT / 2}
                        y1={0}
                        x2={INDENT / 2}
                        y2={geometry.anchors[geometry.anchors.length - 1]}
                        stroke="currentColor"
                        className="text-gray-300"
                        strokeWidth={1}
                    />
                )}
                {geometry.anchors.map((y, i) => (
                    <line
                        key={i}
                        x1={INDENT / 2}
                        y1={y}
                        x2={INDENT}
                        y2={y}
                        stroke="currentColor"
                        className="text-gray-300"
                        strokeWidth={1}
                    />
                ))}
            </svg>

            <div style={{ paddingLeft: INDENT }}>
                {nodes.map((node, i) => (
                    <TreeNodeRow
                        key={node.path}
                        node={node}
                        rowRef={(el) => (rowRefs.current[i] = el)}
                    />
                ))}
            </div>
        </div>
    );
}

function ChevronIcon({ expanded }: { expanded: boolean }) {
    return (
        <svg
            viewBox="0 0 16 16"
            className={`h-3 w-3 shrink-0 text-gray-500 ${expanded ? "rotate-90" : "rotate-0"}`}
            fill="none"
            stroke="currentColor"
            strokeWidth={2}
        >
            <path d="M6 3l5 5-5 5" strokeLinecap="round" strokeLinejoin="round" />
        </svg>
    );
}

function TreeNodeRow({node, rowRef,}: { node: TreeNode; rowRef: (el: HTMLDivElement | null) => void; }) {
    const setSelectedFile = useAppState((state) => state.setSelectedFile);
    const [expanded, setExpanded] = useState(true);

    const isFolder = node.type === "folder";

    return (
        <div>
            <div ref={rowRef} className="flex items-center gap-1" style={{ height: ROW_HEIGHT }}>
                {isFolder ? (
                    <button
                        type="button"
                        onClick={() => setExpanded((e) => !e)}
                        className="flex h-4 w-4 items-center justify-center rounded"
                    >
                        <ChevronIcon expanded={expanded} />
                    </button>
                ) : (<span/>)}

                <span className={"text-gray-200 text-sm " + (isFolder ? "font-bold" : "")}>
                    {isFolder ? node.name : (
                        <button onClick={() => setSelectedFile(node.path)}>
                            {node.name}
                        </button>
                    )}
                </span>
            </div>

            {isFolder && expanded && node.children.length > 0 && (
                <TreeChildren nodes={node.children} />
            )}
        </div>
    );
}

export function Tree({ root }: { root: FolderNode }) {
    return <TreeChildren nodes={root.children} />;
}
