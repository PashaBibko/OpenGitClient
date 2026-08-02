import {useState} from "react";
import {PhotinoBridge} from "../photino.ts";

export function CommitBox() {
    const [ commitName, setCommitName ] = useState("");
    const [ commitDesc, setCommitDesc ] = useState("");

    return (
        <div className="flex flex-col border-gray-950 border-t-6 gap-4 px-4">
            <div/> {/* Creates a gap at the top*/}

            <input
                placeholder="Commit Summary (required)"
                className="border-2 border-gray-200 rounded-md p-2 text-white bg-neutral-600"
                onChange={(e) => setCommitName(e.target.value)}
            />

            <textarea
                placeholder="Commit Description (recommended)"
                className="border-2 border-gray-200 rounded-md p-2 text-white bg-neutral-600 h-36
                           commit-description-scrollbar resize-none"
                onChange={(e) => setCommitDesc(e.target.value)}
            />

            <button
                className={"bg-green-400 px-6 py-2 rounded-md disabled:bg-green-200"}
                disabled={commitName.length === 0}
                onClick={() => {
                    PhotinoBridge.CallBackend("Repo.Commit", {name: commitName, description: commitDesc})
                        .then(_ => { })
                 }}
            >
                Commit
            </button>

            <div/> {/* Creates a gap at the bottom */}
        </div>
    )
}
