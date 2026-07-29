
export function CommitBox() {
    return (
        <div className="flex flex-col border-gray-950 border-t-6 gap-4 px-4">
            <div/> {/* Creates a gap at the top*/}

            <input
                placeholder="Commit Summary (required)"
                className="border-2 border-gray-200 rounded-md p-2 text-white bg-neutral-600"
            />

            <textarea
                placeholder="Commit Description (recommended)"
                className="border-2 border-gray-200 rounded-md p-2 text-white bg-neutral-600 h-36
                           commit-description-scrollbar resize-none"
            />

            <button className="bg-green-400 px-6 py-2 rounded-md">
                Commit
            </button>

            <div/> {/* Creates a gap at the bottom */}
        </div>
    )
}
