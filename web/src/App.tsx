import {TopBar} from "./components/TopBar.tsx";
import {MainContent} from "./components/MainContent.tsx";

export function App() {
    return (
        <div className="bg-neutral-800 h-screen flex flex-col w-screen">
            <TopBar/>
            <MainContent/>
        </div>
    )
}
