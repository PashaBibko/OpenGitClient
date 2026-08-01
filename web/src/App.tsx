import {TopBar} from "./components/TopBar.tsx";
import {MainContent} from "./components/MainContent.tsx";
import {useEffect, useState} from "react";
import {PhotinoBridge} from "./photino.ts";
import {useAppState} from "./AppState.ts";

export function App() {
    const [ hasDoneQuickLoad, setQuickloadState ] = useState<boolean | null>(null);
    const setWorkingDirectory = useAppState((state) => state.setWorkingDir);

    useEffect(() => {
        PhotinoBridge.CallBackend("User.GetLastOpenedRepo").then((path: string) => {
            if (path === "") {
                setQuickloadState(false);
                return;
            }

            const filteredPath = path.replace(/\.git\/$/, "");
            PhotinoBridge.CallBackend("Repo.Open", filteredPath).then((canQuickLoad: boolean) => {
                setQuickloadState(canQuickLoad);

                if (canQuickLoad) {
                    PhotinoBridge.Log(`Quickloading to: "${path}"`);

                    setWorkingDirectory({
                        Filepath: path,
                        IsRepository: true
                    });
                }
            })
        })
    }, []);

    // Doesn't show anything whilst waiting for the backend to respond
    if (hasDoneQuickLoad === null) {
        return (<div/>);
    }

    return (
        <div className="bg-neutral-800 h-screen flex flex-col w-screen">
            <TopBar/>
            <MainContent/>
        </div>
    )
}
