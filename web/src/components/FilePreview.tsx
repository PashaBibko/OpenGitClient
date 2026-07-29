import {PhotinoBridge} from "../photino.ts";
import {useAppState} from "../AppState.ts";
import {useEffect, useState} from "react";

export function FilePreview() {
    const selectedFile = useAppState((state) => state.SelectedFile);
    const [fileContent, setFileContent] = useState<string | null>();

    // Fetches the latest changes of the current file
    useEffect(() => {
        if (selectedFile === null) {
            return; // Nothing to fetch
        }

        let cancelled = false;
        PhotinoBridge.CallBackend("Repo.GetFileDiff", selectedFile).then(raw => {
            if (!cancelled) {
                const content = raw as string;
                setFileContent(content);
            }
        })

        // Stops race conditions when the selected file changes quickly
        return () => {
            cancelled = true;
        }
    })

    if (selectedFile === null) {
        return (
            <div className="text-gray-200">
                No file selected.
            </div>
        )
    }

    return (
        <div className="text-gray-200">
            {fileContent}
        </div>
    )
}
