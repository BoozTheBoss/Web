<!-- src/routes/index.svelte -->
<script>
	let files = [];

	const uploadServerHost = 'http://84.54.23.140:8080/'

	async function getFiles() {
		const response = await fetch(uploadServerHost + 'files');
		const res = await response.json();
		files = res.fileList;
		console.log(files);
	}

	async function handleUpload(event) {
		const selectedFile = event.target.files[0];
		event.preventDefault();
		const formData = new FormData();
		formData.append('uploaded_file', selectedFile);

		const response = await fetch(uploadServerHost + 'uploads', {
			method: 'POST',
			body: formData
		});
		await getFiles();
	}

	async function deleteFromServer(filename) {
		const response = await fetch(uploadServerHost + `files/${filename}`, {
			method: 'DELETE'
		});
		await getFiles();
	}
</script>

<div class="section">
	<div class="container is-max-desktop">
		<hr />
		<h1>File Upload:</h1>
		<br />

		<div class="form-group column">
			<div class="file has-name">
				<label class="file-label">
					<input on:change={handleUpload} type="file" class="file-input" name="uploaded_file" />
					<span class="file-cta">
						<span class="file-icon">
							<i class="fas fa-upload" />
						</span>
						<span class="file-label"> Choose and upload file… </span>
					</span>
				</label>
			</div>
		</div>

		<br />
		<hr />
		<h2>Uploaded Files:</h2>
		<br />

		<table class="table is-fullwidth">
			<thead>
				<tr>
					<th>Name</th>
					<th>Media</th>
					<th>Download</th>
					<th>Delete</th>
				</tr>
			</thead>
			<tbody>
				{#each files as file}
					<tr>
						<td>{file}</td>
						<td>
							<audio controls src= "{uploadServerHost}files/{file}">
								<a class="forceFontColor" href="{uploadServerHost}files/{file}">Download</a>
							</audio>
						</td>
						<td>
							<button class="button is-success">
								<a href="{uploadServerHost}files/{file}" download={file}>Download</a>
							</button>
						</td>
						<td>
							<button on:click={() => deleteFromServer(file)} class="button is-danger">
								Delete
							</button>
						</td>
					</tr>
				{/each}
			</tbody>
		</table>

		{#await getFiles()}
			<p>Loading...</p>
		{:then}
			{#if !files.length}
				<p>No files have been uploaded yet.</p>
			{/if}
		{/await}
	</div>
</div>

<style>
	a {
		color: white;
	}
</style>
