import {useState} from 'react';
import {PhotinoBridge} from "../photino.ts";

function FormatRepoLocation(location: string | null): string {
    if (location === null)
        return "None selected";

    else
        return location;
}

export function TopBar() {
    const [ repoLocation, setRepoLocation ] = useState<string | null>(null);

    return (
        <div className="w-screen border-b-4 border-neutral-950 flex px-4 py-4 h-16 gap-4 text-gray-200 text-md">
            Current repo:
            <div className="text-gray-200 border-2 rounded-md border-neutral-950 px-2">
                {FormatRepoLocation(repoLocation)}
            </div>

            <button
                className="bg-neutral-600 border-2 rounded-md border-neutral-950 px-2 text-md text-gray-200"
                onClick={() => {
                    PhotinoBridge.CallBackend("RepoLocation.Choose").then((res: any) => {
                        setRepoLocation(res as string);
                    })
                }}
            >
                Change
            </button>
        </div>
    )
}
