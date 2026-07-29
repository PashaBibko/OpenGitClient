import {useAppState, WorkingDir} from "../AppState.ts";
import {PhotinoBridge} from "../photino.ts";

function FormatRepoLocation(workingDir: WorkingDir | null): string {
    if (workingDir === null)
        return "None selected";

    else
        return workingDir.Filepath;
}

export function PickRepoButton({ prompt }: { prompt: string}) {
    const setWorkingDirectory = useAppState((state) => state.setWorkingDir);

    return (
        <button
            className="bg-neutral-600 border-2 rounded-md border-neutral-950 px-2 text-md text-gray-200"
            onClick={() => {
                PhotinoBridge.CallBackend("Repo.Choose").then(
                    (res: any) => setWorkingDirectory(res)
                )
            }}
        >{prompt}</button>
    )
}

export function TopBar() {
    const workingDir = useAppState((state) => state.CurrentWorkingDir);

    return (
        <div className="w-screen border-b-6 border-neutral-950 flex px-4 py-4 h-16 gap-4 text-gray-200 text-md">
            Current repo:
            <div className="text-gray-200 border-2 rounded-md border-neutral-950 px-2">
                {FormatRepoLocation(workingDir)}
            </div>

            <PickRepoButton prompt={"Change"}/>
        </div>
    )
}
