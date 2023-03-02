<script>
	import { onMount } from 'svelte';

	const API_URL = 'http://127.0.0.1:1080/';
	let fileList;
	let files = [];

	// `onMount` gets called once at the very start when page is loaded
	onMount(async () => {
		await listFiles();
	});

	// event handler when form is submitted, i.e. "Upload file" button is clicked
	// calls API with FormData - FormData includes each value from <input> fields
	async function uploadFile(event) {
		const formData = new FormData(event.target);

		formData.append('name', files[0].name);

		await fetch(API_URL, {
			method: 'POST',
			body: formData
		});

		// refresh files after we modified filelist in API
		await listFiles();
	}

	// call API to get the current list of files in storage
	async function listFiles() {
		const res = await fetch(API_URL, {
			method: 'GET',
			headers: {
				'Content-Type': 'application/json',
				Accept: 'application/json'
			}
		});
		const resJson = await res.json();
		fileList = resJson.files;
	}

	// call API to delete a specific file by filename
	async function deleteFile(filename) {
		await fetch(`${API_URL}${filename}`, {
			method: 'DELETE'
		});

		// refresh files after we modified filelist in API
		await listFiles();
	}
</script>

<section class="section">
	<!-- upload file: -->
	<form on:submit|preventDefault={uploadFile}>
		<div class="file has-name">
			<label class="file-label">
				<input
					class="file-input"
					id="file-to-upload"
					name="file"
					type="file"
					accept="audio/*"
					bind:files
				/>
				<span class="file-cta">
					<span class="file-icon">
						<i class="fas fa-upload" />
					</span>
					<span class="file-label"> Choose a file… </span>
				</span>
				<span class="file-name">
					{#if files.length > 0}
						{files[0].name}
					{:else}
						no file selected ...
					{/if}
				</span>
			</label>
		</div>

		<button class="button" type="submit"> Upload file </button>
	</form>

	<!-- file list -->
	<div class="">
		{#if fileList}
			<ol>
				{#each fileList as filename}
					<figure>
						<figcaption>{filename}</figcaption>
						<audio controls src="{API_URL}{filename}" />
						<button class="button" on:click={() => deleteFile(filename)}>
							DELETE {filename}
						</button>
					</figure>
				{/each}
			</ol>
		{:else}
			<p>No files yet</p>
		{/if}
	</div>
</section>
