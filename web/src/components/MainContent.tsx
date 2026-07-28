import {useAppState} from "../AppState.ts";
import {PhotinoBridge} from "../photino.ts";

import {useState, useEffect} from "react";

interface FileStatusBreakdown {
    FileLocation: string;
    SpecialState: string;
    StagedState: string;
    UnstagedState: string;
}

export function MainContent() {
    const [ fileChanges, setFileChanges ] = useState<FileStatusBreakdown[] | null>(null);
    const workingDir = useAppState((state) => state.CurrentWorkingDir);

    // Makes sure it has the correct file changes stored for the current repository
    useEffect(() => {
        if (workingDir === null || !workingDir.IsRepository) {
            return; // Nothing to fetch, no valid repository
        }

        let cancelled = false; // Makes sure that it does not accidentally overwrite a newer version
        PhotinoBridge.CallBackend("Repo.GetChanges").then(raw => {
            if (!cancelled) {
                setFileChanges(raw as FileStatusBreakdown[]);
            }
        })

        return () => {
            cancelled = true;
        }
    }, [ workingDir?.Filepath, workingDir?.IsRepository ])

    // Gets the working dir and checks if it is a repository and displays relevant screen if it isn't
    if (workingDir === null || !workingDir.IsRepository) {
        return (
            <div className="flex flex-1 text-gray-200 text-2xl justify-center text-center h-max items-center">
                <div className="flex flex-col">
                    <div className="text-gray-200 text-3xl">
                        This directory does not contain a git repository.
                    </div>

                    <div className="text-gray-400 text-xl">
                        Would you like to create one? [Not yet implemented]
                    </div>
                </div>
            </div>
        );
    }

    // Returns an empty div whilst waiting for the file changes
    // Should only show for a fraction of a second so no loading state needs to be shown
    if (fileChanges === null) {
        return (<div/>);
    }

    return (
        <div>
            {fileChanges.map((fileInfo: FileStatusBreakdown) => (
                <div key={fileInfo.FileLocation} className="text-gray-200">
                    <span className="font-bold">
                        {fileInfo.FileLocation}:
                    </span>

                     <div className="w-20"/>

                    {fileInfo.SpecialState !== "none"
                        ? fileInfo.SpecialState
                        : fileInfo.StagedState + " | " + fileInfo.UnstagedState
                    }
                </div>
            ))}
        </div>
    )
}
