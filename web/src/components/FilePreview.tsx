import {useAppState} from "../AppState.ts";

export function FilePreview() {
    const selectedFile = useAppState((state) => state.SelectedFile);

    if (selectedFile === null) {
        return (
            <div className="text-gray-200">
                No file selected.
            </div>
        )
    }

    return (
        <div className="text-gray-200">
            Previewing {selectedFile}
        </div>
    )
}
