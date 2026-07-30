import {TopBar} from "./components/TopBar.tsx";
import {MainContent} from "./components/MainContent.tsx";
import {useEffect, useState} from "react";
import {PhotinoBridge} from "./photino.ts";
import {useAppState} from "./AppState.ts";

export function App() {
    const [ hasDoneQuickLoad, setQuickloadState ] = useState<boolean | null>(null);
    const setWorkingDirectory = useAppState((state) => state.setWorkingDir);

    useEffect(() => {
        const path: string = "D:/repos/projects/OpenGitClient/.git/";
        PhotinoBridge.CallBackend("Repo.Open", path).then((res: any) => {
            const canQuickLoad = res as boolean;
            console.log(canQuickLoad);

            setQuickloadState(canQuickLoad);

            if (canQuickLoad) {
                setWorkingDirectory({
                    Filepath: path,
                    IsRepository: true
                });
            }
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
