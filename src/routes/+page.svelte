<script>
	import { onMount } from 'svelte';

	let fileList;

	async function uploadFile(event) {
		const formData = new FormData(event.target);

		await fetch('/upload', {
			method: 'POST',
			body: formData
		});

		await listFiles();
	}

	async function listFiles() {
		const lisdir = await fetch('/upload', {
			method: 'GET',
			headers: {
				'Content-Type': 'application/json',
				Accept: 'application/json'
			}
		});
		const res = await lisdir.json();
		fileList = res.dir;
		return lisdir;
	}
	onMount(async () => {
		await listFiles();
	});

	async function delteFile(filename) {
		const formData = new FormData();

		formData.append("filename", filename)
		await fetch('/upload', {
			method: 'DELETE',
			body: formData
		});

		await listFiles();
	}
</script>

<!-- upload file: -->
<form on:submit|preventDefault={uploadFile}>
	<input class="hidden" id="file-to-upload" name="audio" type="file" accept="audio/*" />
	<button type="submit">Upload file</button>
</form>

<!-- Tabelle: -->
{#if fileList}
	<ol>
		{#each fileList as filename}
			<figure>
				<figcaption>{filename}</figcaption>
				<audio controls src="audio/{filename}">
					<!-- <a href="audio/{filename}">Download audio</a> -->
				</audio>
				<button on:click={() => delteFile(filename)}>DELETE {filename}</button> <!-- fun aufruf mit filename : -->
			</figure>
		{/each}
	</ol>
{:else}
	<p>No files yet</p>
{/if}
