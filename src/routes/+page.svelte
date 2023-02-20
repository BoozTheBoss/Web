<script>
	import { onMount } from 'svelte';
	import Uppy from '@uppy/core';
  import XHRUpload from '@uppy/xhr-upload';
  import Dashboard from '@uppy/dashboard';

  let uppy;

  function initializeUppy() {
    uppy = new Uppy({
      id: 'uppy',
      autoProceed: true,
      restrictions: {
        maxFileSize: 1000000000, // set the maximum file size
        maxNumberOfFiles: 10 // set the maximum number of files
      }
    })
    .use(XHRUpload, {
      endpoint: '/upload',
      fieldName: 'file',
      formData: true
    })
    .use(Dashboard, {
      id: 'dashboard',
      target: '#uppy',
      proudlyDisplayPoweredByUppy: false
    });
  }

  function destroyUppy() {
    uppy.close();
  }

	let fileList;
	let files = [];

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

		formData.append('filename', filename);
		await fetch('/upload', {
			method: 'DELETE',
			body: formData
		});

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
					name="audio"
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

		<button class="button" type="submit">Upload file</button>
	</form>

	<div class="">
		<!-- Tabelle: -->
		{#if fileList}
			<ol>
				{#each fileList as filename}
					<figure>
						<figcaption>{filename}</figcaption>
						<audio controls src="audio/{filename}">
							<!-- <a href="audio/{filename}">Download audio</a> -->
						</audio>
						<button class="button" on:click={() => delteFile(filename)}>DELETE {filename}</button>
						<!-- fun aufruf mit filename : -->
					</figure>
				{/each}
			</ol>
		{:else}
			<p>No files yet</p>
		{/if}
	</div>
</section>


<div on:destroy={destroyUppy}>
	<div id="uppy"></div>
  </div>
  
  <button on:click={initializeUppy}>Select Files</button>
