import {PhotinoBridge} from "../photino.ts";
import {useAppState} from "../AppState.ts";
import {useEffect, useState} from "react";

export function FilePreview() {
    const selectedFile = useAppState((state) => state.SelectedFile);
    const [fileContent, setFileContent] = useState<string[] | null>();

    // Fetches the latest changes of the current file
    useEffect(() => {
        if (selectedFile === null) {
            return; // Nothing to fetch
        }

        let cancelled: boolean = false;
        PhotinoBridge.CallBackend("Repo.GetFileDiff", selectedFile).then(raw => {
            if (!cancelled) {
                const content = raw as string[];
                setFileContent(content);
            }
        })

        // Stops race conditions when the selected file changes quickly
        return () => {
            cancelled = true;
        }
    }, [selectedFile]);

    // Keeps no file selected message whilst loading the file to avoid flashes
    if (selectedFile === null || fileContent === null || fileContent === undefined) {
        return (
            <div className="text-gray-200">
                No file selected.
            </div>
        )
    }

    return (
        <div className="text-gray-200">
            {fileContent.map((line, index) => (
                <div key={index} className="whitespace-pre-wrap">
                    {line}
                </div>
            ))}
        </div>
    )
}
