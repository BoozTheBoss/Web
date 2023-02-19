<script>
	import { onMount } from 'svelte';
	

	let fileInput;
	let files;
	let avatar;
	let fileList = [];

	// function getBase64(image) {
	// 	const reader = new FileReader();
	// 	reader.readAsDataURL(image);
	// 	reader.onload = (e) => {
	// 		avatar = e.target.result;
	// 		uploadFunction(e.target.result);
	// 	};
	// }

	async function uploadFunction(event) {
		console.log(files);

		await fetch(`/upload`, {
			method: 'POST',
			headers: {
				'Content-Type': 'multipart/form-data'
			},
			body: files
		});
	}

	async function uploadFile(file) {
    const fileReader = new FileReader();

    fileReader.onload = function () {
      const xhr = new XMLHttpRequest();
      xhr.open('POST', '/upload', true);
      xhr.setRequestHeader('X-Filename', file.name);
      xhr.send(this.result);
    };

    fileReader.readAsArrayBuffer(file);
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
		// res = { ok: 200, dir: [...] }
		fileList = res.dir;
		return lisdir;
	}
	onMount(async () => {
		await listFiles();
	});
</script>


<!-- upload file: -->
<form method="post" action="/upload" enctype="multipart/form-data">
	<!-- <input
		class="hidden"
		id="file-to-upload"
    	name="audio"
		type="file"
		accept="audio/*"
		bind:files
		bind:this={fileInput}
	/> -->
	<input type="file" on:change="{(event) => uploadFile(event.target.files[0])}">
	<button type="submit">Upload file</button>
</form>


<!-- Tabelle: -->
<ol>
	{#each fileList as filename}
		<!-- <li>{filename}</li>
		<a href="audio/{filename}">Download {filename}</a> -->
		<figure>
			<figcaption>{filename}</figcaption>
			<audio controls src="audio/{filename}">
				<a href="audio/{filename}"> Download audio </a>
			</audio>
		</figure>
	{/each}
</ol>
